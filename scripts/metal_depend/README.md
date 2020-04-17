# metal_depend

This is a python tool based on pydevicetree
([GitHub](https://github.com/sifive/pydevicetree)/[PyPI](https://pypi.org/project/pydevicetree/))
which computes the set of FreedomMetal sources needed on a particular platform.

## Usage

```
usage: metal_depend.py [-h] -d DTS -o OUTPUT

Compute freedom-metal depedencies from Devicetrees

optional arguments:
  -h, --help            show this help message and exit
  -d DTS, --dts DTS     The path to the Devicetree for the target
  -o OUTPUT, --output OUTPUT
                        The path of the linker script file to output
```

## Required Devicetree Properties

This source dependency generator expects that the Devicetree has annotated
each block with the compatible hardware definition. That property is:

  - `compatible`, which describes which interface this block supports

Each of these properties is a `string` that describes the interface supported.

For example, the chosen node may include the following property:
```
compatible = "sifive,fe310-g000,hfxosc";
```
This maps to a driver named sifive_fe310-g000_hfxosc.c, which is found
in freedom-metal/src/drivers

## Example Invocation

```
$ ./metal_depend.py -d e31.dts -o metal.mk
Discovering source dependencies for e31.dts

$ head -n 20 metal.mk
```

## Copyright and License

Copyright (c) 2020 SiFive Inc.

The contents of this repository are distributed according to the terms described in the LICENSE
file.
