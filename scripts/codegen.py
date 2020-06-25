#!/usr/bin/env python3
# Copyright (c) 2020 SiFive Inc.
# SPDX-License-Identifier: Apache-2.0

import argparse
import glob 
import os
import sys

import jinja2
import pydevicetree

METAL_APIS = [
    "clock"
    "cpu",
    "interrupt",
    "shutdown",
    "uart",
]

DEFAULT_TEMPLATE_PATHS = [
    "templates",
]

DEFAULT_CLOCK_DRIVERS = [
    "fixed-clock",
    "sifive,fe310-g000,hfrosc",
    "sifive,fe310-g000,hfxosc",
    "sifive,fe310-g000,lfrosc",
    "sifive,fe310-g000,pll",
]

DEFAULT_INTERRUPT_DRIVERS = [
    "riscv,cpu-intc",
    "riscv,plic0",
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

    arg_parser.add_argument("--uart-driver", default="sifive,uart0",
            help="The driver for the UART API")
    arg_parser.add_argument("--gpio-driver", default="sifive,gpio0",
            help="The driver for the GPIO API")
    arg_parser.add_argument("--shutdown-driver", default="sifive,test0",
            help="The driver for the Shutdown API")

    arg_parser.add_argument("--clock-drivers",
            nargs='*',
            default=DEFAULT_CLOCK_DRIVERS,
            help="The drivers for the clock API")
    arg_parser.add_argument("--interrupt-drivers",
            nargs='*',
            default=DEFAULT_INTERRUPT_DRIVERS,
            help="The drivers for the interrupt API")

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

def assign_ids(dts, args):
    drivers = []
    drivers.append(args.uart_driver)
    drivers.append(args.gpio_driver)
    drivers.append(args.shutdown_driver)
    drivers += args.clock_drivers
    drivers += args.interrupt_drivers

    for driver in drivers:
        for node_id, node in enumerate(dts.match(driver)):
            driver_ids[node] = node_id

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

def render_templates(template_dirs, args, template_data):
    templates = []
    for d in template_dirs:
        templates += glob.glob("{}/metal/*.h".format(d))
        templates += glob.glob("{}/metal/generated/*.h".format(d))
        templates += glob.glob("{}/metal/machine/*.h".format(d))

    for template in templates:
        template =  template.replace("templates/", "")
        output_file = "{}/{}".format(args.output_dir, template)
        dirname = os.path.dirname(output_file)
        if not os.path.exists(dirname):
            os.makedirs(dirname)
        with open(output_file, 'w') as out:
            out.write(get_template(template).render(template_data))

def main():
    args = parse_arguments(sys.argv[1:])

    dts = pydevicetree.Devicetree.parseFile(
            args.dts, followIncludes=True)

    # Assign driver IDs to all device instances
    assign_ids(dts, args)

    devices = []
    devices.append(args.uart_driver)
    devices.append(args.gpio_driver)
    devices.append(args.shutdown_driver)
    devices += args.clock_drivers
    devices += args.interrupt_drivers

    # Convert the Devicetree object tree into dictionary data
    # which can be rendered by the templates
    template_data = {
        'harts' : [node_to_dict(hart, dts) for hart in dts.match("^riscv$")],
        'uarts' : [node_to_dict(uart, dts) for uart in dts.match(args.uart_driver)],
        'gpios' : [node_to_dict(gpio, dts) for gpio in dts.match(args.gpio_driver)],
        'devices' : devices,
    }

    for driver in args.clock_drivers:
        key = to_snakecase(driver) + 's'
        template_data[key] = [node_to_dict(clock, dts) for clock in dts.match(driver)]

    for driver in args.interrupt_drivers:
        key = to_snakecase(driver) + 's'
        template_data[key] = [node_to_dict(controller, dts) for controller in dts.match(driver)]

    import pprint
    pprint.pprint(template_data)

    render_templates(args.template_paths, args, template_data)

if __name__ == "__main__":
    main()
