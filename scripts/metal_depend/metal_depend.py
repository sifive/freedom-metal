#!/usr/bin/env python3
# Copyright (c) 2020 SiFive Inc.
# SPDX-License-Identifier: Apache-2.0

"""Generate list of freedom-metal drivers from devicetree source files"""

import argparse
import sys
import os

import jinja2
import pydevicetree

from sources import get_sources, find_source

TEMPLATES_PATH = "templates"

def missingvalue(message):
    """
    Raise an UndefinedError
    This function is made available to the template so that it can report
    when required values are not present and cause template rendering to
    fail.
    """
    raise jinja2.UndefinedError(message)


def parse_arguments(argv):
    """Parse the arguments into a dictionary with argparse"""
    arg_parser = argparse.ArgumentParser(
        description="Generate linker scripts from Devicetrees")

    arg_parser.add_argument("-d", "--dts", required=True,
                            help="The path to the Devicetree for the target")
    arg_parser.add_argument("-m", "--metal", required=True,
                            help="The path to the freedom-metal source tree")
    arg_parser.add_argument("-f", "--features",
                            help="A comma-separated list of metal features to enable")
    arg_parser.add_argument("-o", "--output",
                            help="The path of the source list file to output")
    arg_parser.add_argument("--template",
                            help="Emits a source list using the specified template (default=make)")

    return arg_parser.parse_args(argv)


def get_template(parsed_args):
    """Initialize jinja2 and return the right template"""
    env = jinja2.Environment(
        loader=jinja2.PackageLoader(__name__, TEMPLATES_PATH),
        trim_blocks=True, lstrip_blocks=True,
    )
    # Make the missingvalue() function available in the template so that the
    # template fails to render if we don't provide the values it needs.
    env.globals["missingvalue"] = missingvalue

    template_name = parsed_args.template
    if not template_name:
        template_name = "make"

    template = env.get_template("%s.mk" % template_name)
    print("Generating source list with %s template" % template_name, file=sys.stderr)

    return template


METAL_DIRS = ('src', 'src/drivers')

HELPERS_C = (
    ("uart0", "uart.c"),
    ("wdog0", "watchdog.c"),
    ("clic0", "interrupt.c"),
    ("plic0", "interrupt.c"),
    ("clint0", "interrupt.c"),
    ("gpio0", "gpio.c"),
    )

METAL_C = ('init.c', 'synchronize_harts.c', 'inline.c', 'cpu.c', 'clock.c', 'timer.c')

METAL_S = ('entry.S', 'trap.S', 'vector.S')

def find_dirs(files):
    """Find the set of directories containing files"""
    ret = []
    for file in files:
        direct = os.path.dirname(file)
        if direct not in ret:
            ret += [direct]
    ret.sort()
    return ret

def find_paths(names, dirs):
    """Find the full paths for the given names"""
    ret = []
    for name in names:
        path = find_source(name, dirs)
        if path and path not in ret:
            ret += [path]
    ret.sort()
    return ret

FEATURE_C = {
    "stdio": ['tty.c'],
    "exit": ['shutdown.c'],
}

def find_features(feature_arg, dirs):
    """Get the sources needed for the requested features"""

    # Convert comma-separated argument into list
    features = ()
    if feature_arg:
        features = feature_arg.split(',')

    feature_c = []

    for feature in features:
        if feature in FEATURE_C:
            feature_c += find_paths(FEATURE_C[feature], dirs)

    return feature_c

def main(argv):
    """Parse arguments, extract data, and render the linker script to file"""
    parsed_args = parse_arguments(argv)

    template = get_template(parsed_args)

    dts = pydevicetree.Devicetree.parseFile(
        parsed_args.dts, followIncludes=True)

    metal = parsed_args.metal

    dirs = [os.path.join(metal, sub) for sub in METAL_DIRS]

    (target_c, target_s) = get_sources(dts, dirs)

    # Add helper sources based on pattern matching source filenames

    helpers_c = []
    for t_c in target_c:
        for helper_c in HELPERS_C:
            if helper_c[0] in t_c:
                helpers_c += [helper_c[1]]

    target_c += find_paths(helpers_c, dirs)

    # Add sources to support requested features

    target_c += find_features(parsed_args.features, dirs)

    # Add required metal sources

    target_c += find_paths(METAL_C, dirs)
    target_s += find_paths(METAL_S, dirs)

    # Pass sorted sources to the template generator so that the generated
    # file is reproducible.

    target_c.sort()
    target_s.sort()

    target_c_dir = find_dirs(target_c)
    target_s_dir = find_dirs(target_s)

    values = {
        "target_c": target_c,
        "target_s": target_s,
        "target_c_dir": target_c_dir,
        "target_s_dir": target_s_dir
    }

    if parsed_args.output:
        with open(parsed_args.output, 'w', encoding='utf-8') as outfile:
            outfile.write(template.render(values))
    else:
        print(template.render(values))


if __name__ == "__main__":
    main(sys.argv[1:])
