#!/usr/bin/env python3
# Copyright (c) 2020 SiFive Inc.
# SPDX-License-Identifier: Apache-2.0

import argparse
import glob 
import os
import sys

import jinja2
import pydevicetree

def parse_arguments(argv):
    arg_parser = argparse.ArgumentParser(
            description="Generate Freedom Metal code from the target Devicetree")

    arg_parser.add_argument("-d", "--dts",
            required=True,
            help="The path to the target Devicetree")
    arg_parser.add_argument("-o", "--output-dir",
            required=True,
            help="The path to the directory to output generated code")

    return arg_parser.parse_args(argv)

def get_template(template):
    env = jinja2.Environment(
            loader=jinja2.FileSystemLoader("templates"),
            trim_blocks=True, lstrip_blocks=True)
    env.globals['to_snakecase'] = to_snakecase

    return env.get_template(template)

def to_snakecase(s):
    return s.lower().replace(',', '_').replace('-', '_')

def render_templates(args, template_data):
    templates = [
        "templates/src/interrupt_table.c",
        "templates/src/jump_table.S",
    ]

    for template in templates:
        template = template.replace("templates/", "")
        output_file = "{}/{}".format(args.output_dir, template)
        dirname = os.path.dirname(output_file)
        if not os.path.exists(dirname):
            os.makedirs(dirname)
        with open(output_file, 'w') as out:
            out.write(get_template(template).render(template_data))

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
        

def main():
    args = parse_arguments(sys.argv[1:])

    dts = pydevicetree.Devicetree.parseFile(
            args.dts, followIncludes=True)

    # Convert the Devicetree object tree into dictionary data
    # which can be rendered by the templates
    template_data = {
        'local_interrupts' : local_interrupts(dts),
        'global_interrupts' : global_interrupts(dts),
    }

    import pprint
    pprint.pprint(template_data)

    render_templates(args, template_data)

if __name__ == "__main__":
    main()
