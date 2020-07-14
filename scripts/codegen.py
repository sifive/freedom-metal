#!/usr/bin/env python3
# Copyright (c) 2020 SiFive Inc.
# SPDX-License-Identifier: Apache-2.0

import argparse
import configparser
import glob 
import os
import pprint
import sys

import jinja2
import pydevicetree

DEFAULT_SOURCE_PATHS = [
    ".",
    "sifive-blocks",
]

def parse_arguments(argv):
    arg_parser = argparse.ArgumentParser(
            description="Generate Freedom Metal code from the target Devicetree")

    arg_parser.add_argument("-d", "--dts",
            required=True,
            help="The path to the target Devicetree")
    arg_parser.add_argument("-o", "--output-dir",
            required=True,
            help="The path to the directory to output generated code")

    arg_parser.add_argument("--source-paths",
            nargs='*',
            default=DEFAULT_SOURCE_PATHS,
            help="The paths to look for template")

    args =  arg_parser.parse_args(argv)

    args.template_paths = [ d + "/templates" for d in args.source_paths ]

    return args


def get_template(template, args):
    env = jinja2.Environment(
            loader=jinja2.ChoiceLoader([jinja2.FileSystemLoader(d) for d in args.template_paths]),
            trim_blocks=True, lstrip_blocks=True)
    env.globals['to_snakecase'] = to_snakecase
    env.filters['rootname'] = rootname

    return env.get_template(template)

def to_snakecase(s):
    return s.lower().replace(',', '_').replace('-', '_').replace('.', '_')

def rootname(path):
    return os.path.splitext(os.path.basename(path))[0]

def unique_irqs(irqs):
    """
    Takes a dictionary containing the list of interrupt lines and filters them to just
    the unique interrupt sources.

    This function is made available to the template environment so that interrupt handler
    declarations and definitions aren't repeated when multiple instances of a single IP block exist.
    """
    filtered_irqs = []
    for irq in irqs:
        if all([irq['source'] != i['source'] for i in filtered_irqs]):
            filtered_irqs.append(irq)
    return filtered_irqs

driver_ids = dict()

def assign_ids(dts, devices):
    for api in devices:
        for device in devices[api]:
            for node_id, node in enumerate(dts.match(device)):
                driver_ids[node] = node_id

def local_interrupts(dts):
    irqs = []
    
    for node in dts.match("sifive,local-external-interrupts0"):
        for source_id, irq_id in enumerate(node.get_fields("interrupts")):
            irqs.append({
                'source': {
                    'compatible': "sifive,local-external-interrupts0",
                    'id': source_id,
                },
                'id': irq_id,
            })

    irqs.sort(key=lambda x: x['id'])

    local_interrupts = {
        'irqs': irqs,
    }

    return local_interrupts

def global_interrupts(dts):
    def is_global_int_source(node):
        ref = node.get_field("interrupt-parent")
        if ref == None:
            return False
        intc = dts.get_by_reference(ref)
        return intc.get_field("compatible") == "riscv,plic0"
    
    plic_sources = dts.filter(is_global_int_source)

    irqs = []

    for node in plic_sources:
        for source_id, irq_id in enumerate(node.get_fields("interrupts")):
            irqs.append({
                'source': {
                    'compatible': node.get_field("compatible"),
                    'id': source_id,
                },
                'id': irq_id,
            })

    irqs.sort(key=lambda x: x['id'])

    global_interrupts = {
        'irqs': irqs,
    }

    return global_interrupts

def resolve_phandles(dts):
    import pdb

    for node in dts.all_nodes():
        for prop in node.properties:
            if prop.name == "interrupt-parent":
                phandle = node.get_field("interrupt-parent")
                if isinstance(phandle, int):
                    parent = dts.filter(lambda n: n.get_field("phandle") == phandle)[0]
                    del node.properties[node.properties.index(prop)]
                    node.properties.append(pydevicetree.ast.Property.from_dts("interrupt-parent = <&{" + parent.get_path() + "}>;"))
            if prop.name == "interrupts-extended":
                int_ext = node.get_fields("interrupts-extended")
                if isinstance(int_ext[0], int):
                    property_string = "interrupts-extended = <"
                    for phandle, irq in zip(int_ext[::2], int_ext[1::2]):
                        parent = dts.filter(lambda n: n.get_field("phandle") == phandle)[0]
                        property_string += "&{" + parent.get_path() + "} " + str(irq) + " "
                    property_string += ">;"
                    del node.properties[node.properties.index(prop)]
                    node.properties.append(pydevicetree.ast.Property.from_dts(property_string))

def is_reference(x):
    return isinstance(x, (pydevicetree.ast.LabelReference, pydevicetree.ast.PathReference))

def node_to_dict(node, dts):
    d = dict()
    for prop in node.properties:
        key = to_snakecase(prop.name)

        values = []
        if key == "reg":
            if is_reference(prop.values[0]):
                # When the reg property looks like
                #  reg = <&aon 0x70 &aon 0x73>;
                # The pairs of Node References and offsets means
                #  1. Look up the control registers of the referenced node
                #  2. Add the offset to the base address
                references = prop.values[0::2] # [&aon, &aon]
                offsets = prop.values[1::2]    # [0x70, 0x7C]
                for ref, offset in zip(references, offsets):
                    values.append([dts.get_by_reference(ref).get_reg()[0][0] + offset, 0])
            else:
                values = node.get_reg()
        else:
            for value in prop.values:
                if is_reference(value):
                    values.append(node_to_dict(dts.get_by_reference(value), dts))
                else:
                    values.append(value)
        d[key] = values

    if node in driver_ids:
        d['id'] = driver_ids[node]

    if 'clock_names' in d:
        clocks = dict()
        for idx, name in enumerate(d['clock_names']):
            clocks[name] = d['clocks'][idx]
        d['clocks_by_name'] = clocks

    if 'reg_names' in d:
        regs = dict()
        for idx, name in enumerate(d['reg_names']):
            if is_reference(node.get_field("reg")):
                regs[name] = d['reg'][idx]
            else:
                regs[name] = node.get_reg()[idx]
        d['regs_by_name'] = regs

    return d

def get_templates(template_paths, strip_dir=True):
    templates = []
    for d in template_paths:
        if strip_dir:
            templates += [g.replace(d + "/", "") for g in glob.iglob("{}/**/*.j2".format(d), recursive=True)]
        else:
            templates += [g for g in glob.iglob("{}/**/*.j2".format(d), recursive=True)]

    return templates

def get_c_sources(args):
    sources = []
    for d in args.source_paths:
        sources += [g for g in glob.glob("{}/src/**/*.c".format(d), recursive=True)]
    sources += [t.replace(".j2", "") for t in get_templates(args.template_paths) if ".c" in t]

    return sources

def get_asm_sources(args):
    sources = []
    for d in args.source_paths:
        sources += [g for g in glob.glob("{}/src/**/*.S".format(d), recursive=True)]
    sources += [t.replace(".j2", "") for t in get_templates(args.template_paths) if ".S" in t]

    return sources

def get_source_dirs(args):
    dirs = []
    for d in args.source_paths:
        dirs += [os.path.join(d, 'src')]
        for root, subdirs, files in os.walk(os.path.join(d, 'src')):
            dirs += [os.path.join(root,d) for d in subdirs]
    return dirs

def render_templates(args, template_data):
    for template in get_templates(args.template_paths):
        output_file = "{}/{}".format(args.output_dir, template)
        output_file = output_file.replace(".j2", "")
        dirname = os.path.dirname(output_file)
        if not os.path.exists(dirname):
            os.makedirs(dirname)

        with open(output_file, 'w') as out:
            print("Rendering template {}".format(template))
            out.write(get_template(template, args).render(template_data))

def get_devices_from_manifests(template_paths):
    devices = dict()

    for d in template_paths:
        try:
            with open("{}/MANIFEST.ini".format(d), 'r') as manifest:
                print("Loading templates from \"{}\"".format(d))
                config = configparser.ConfigParser()
                config.read_file(manifest)

                for api in config.sections():
                    if api not in devices:
                        devices[api] = []
                    if api in config:
                        devices[api] += config[api]['compatible'].split()
        except FileNotFoundError:
            sys.stderr.write("ERROR: Template path {} does not contain MANIFEST.ini\n".format(d))

    print("Template directories contain support for the following devices:")
    for api in devices:
        print(api)
        for device in devices[api]:
            print("\t- {}".format(device))

    return devices


def main():
    args = parse_arguments(sys.argv[1:])

    dts = pydevicetree.Devicetree.parseFile(
            args.dts, followIncludes=True)

    resolve_phandles(dts)

    # Get list of supported devices from template manifests
    devices = get_devices_from_manifests(args.template_paths)

    # Assign driver IDs to all device instances
    assign_ids(dts, devices)

    # Convert the Devicetree object tree into dictionary data
    # which can be rendered by the templates
    template_data = {
        'chosen' : node_to_dict(dts.get_by_path("/chosen"), dts),
        'harts' : [node_to_dict(hart, dts) for hart in dts.match("^riscv$")],
        'local_interrupts' : local_interrupts(dts),
        'global_interrupts' : global_interrupts(dts),
        'devices' : dict(),
        'default_drivers' : dict(),
        'devicetree_path' : args.dts,
        'c_sources' : get_c_sources(args),
        'asm_sources' : get_asm_sources(args),
        'source_dirs' : get_source_dirs(args),
        'templates' : get_templates(args.source_paths, strip_dir=False),
        'source_paths' : args.source_paths,
    }

    for api in devices:
        for compat in devices[api]:
            nodes = [node_to_dict(node, dts) for node in dts.match(compat)]
            if len(nodes) > 0:
                print("Found {} instances of device {}".format(len(nodes), compat))
                template_data['devices'][compat] = nodes
                template_data['default_drivers'][api] = compat
                
    if 'stdout_path' in  template_data['chosen']:
        path, baud = template_data['chosen']['stdout_path'][0].split(':')
        node = dts.get_by_path(path)
        template_data['chosen']['stdout_path'] = [node_to_dict(node, dts), baud]
        template_data['default_drivers']['uart'] = node.get_field("compatible")
               

    with open("{}/template_data.log".format(args.output_dir), "w") as log:
        log.write(pprint.pformat(template_data))

    render_templates(args, template_data)

if __name__ == "__main__":
    jinja2.filters.FILTERS['unique_irqs'] = unique_irqs
    main()
