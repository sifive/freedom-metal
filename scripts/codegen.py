#!/usr/bin/env python3
# Copyright (c) 2020 SiFive Inc.
# SPDX-License-Identifier: Apache-2.0

import argparse
import configparser
import glob 
import os
import sys

import jinja2
import pydevicetree

METAL_APIS = [
    "clock",
    "cpu",
    "interrupt",
    "shutdown",
    "uart",
    "gpio",
]

DEFAULT_TEMPLATE_PATHS = [
    "templates",
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

    arg_parser.add_argument("--template-paths",
            nargs='*',
            type=list,
            default=DEFAULT_TEMPLATE_PATHS,
            help="The paths to look for template")

    return arg_parser.parse_args(argv)

def get_template(template):
    env = jinja2.Environment(
            loader=jinja2.FileSystemLoader("templates"),
            trim_blocks=True, lstrip_blocks=True)
    env.globals['to_snakecase'] = to_snakecase

    return env.get_template(template)

def to_snakecase(s):
    return s.lower().replace(',', '_').replace('-', '_')

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

def node_to_dict(node, dts):
    d = dict()
    for prop in node.properties:
        key = to_snakecase(prop.name)

        values = []
        if key == "reg" and isinstance(prop.values[0], pydevicetree.ast.LabelReference):
            # When the reg property looks like
            #  reg = <&aon 0x70 &aon 0x73>;
            # The pairs of Node References and offsets means
            #  1. Look up the control registers of the referenced node
            #  2. Add the offset to the base address
            references = prop.values[0::2] # [&aon, &aon]
            offsets = prop.values[1::2]    # [0x70, 0x7C]
            for ref, offset in zip(references, offsets):
                values.append(dts.get_by_reference(ref).get_reg()[0][0] + offset)
        else:
            for value in prop.values:
                if isinstance(value, pydevicetree.ast.LabelReference):
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
            regs[name] = d['reg'][idx]
        d['regs_by_name'] = regs

    return d

def render_templates(template_paths, args, template_data):
    templates = []
    for d in template_paths:
        templates += glob.glob("{}/metal/*.j2".format(d))
        templates += glob.glob("{}/metal/generated/*.j2".format(d))
        templates += glob.glob("{}/metal/machine/*.j2".format(d))
        templates += glob.glob("{}/src/*.j2".format(d))

    for template in templates:
        template =  template.replace("templates/", "")
        output_file = "{}/{}".format(args.output_dir, template)
        output_file.replace(".j2", "")
        dirname = os.path.dirname(output_file)
        if not os.path.exists(dirname):
            os.makedirs(dirname)
        with open(output_file, 'w') as out:
            out.write(get_template(template).render(template_data))

def get_devices_from_manifests(template_paths):
    devices = dict()

    for d in template_paths:
        with open("{}/MANIFEST.ini".format(d), 'r') as manifest:
            config = configparser.ConfigParser()
            config.read_file(manifest)

            for api in METAL_APIS:
                if api not in devices:
                    devices[api] = []
                if api in config:
                    devices[api] += config[api]['compatible'].split()

    print(devices)

    return devices


def main():
    args = parse_arguments(sys.argv[1:])

    dts = pydevicetree.Devicetree.parseFile(
            args.dts, followIncludes=True)

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
        'devices' : devices,
    }

    if 'stdout_path' in  template_data['chosen']:
        path, baud = template_data['chosen']['stdout_path'][0].split(':')
        node = dts.get_by_path(path)
        template_data['chosen']['stdout_path'] = [node_to_dict(node, dts), baud]
               
    for api in METAL_APIS:
        for device in devices[api]:
            template_data[to_snakecase(device + 's')] = [node_to_dict(node, dts) for node in dts.match(device)]
            template_data[api + 's'] = [node_to_dict(node, dts) for node in dts.match(device)]

    import pprint
    pprint.pprint(template_data)

    render_templates(args.template_paths, args, template_data)

if __name__ == "__main__":
    main()
