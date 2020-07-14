# Copyright © 2020 Keith Packard #
# SPDX-License-Identifier: Apache-2.0 #

.DEFAULT_GOAL = $(PROGRAM)

RISCV_LIBC ?= picolibc

ifeq ($(RISCV_LIBC),picolibc)
include $(FREEDOM_METAL)/metal_pico.make
endif

ifeq ($(RISCV_LIBC),nano)
include $(FREEDOM_METAL)/metal_nano.make
endif

ifeq ($(CONFIGURATION),release)
OPT ?= -Os -g
else
OPT ?= -O0 -g
endif

ifneq ($(TARGET),)
DEVICETREE = ../../bsp/$(TARGET)/design.dts
endif

include metal/metal.mk

vpath %.S metal/src:$(METAL_SRC_PATH):$(METAL_HELPER_VPATH)
vpath %.c metal/src:$(METAL_SRC_PATH):$(METAL_HELPER_VPATH)

include metal/settings.mk

CC = riscv64-unknown-elf-gcc

ifeq ($(LDSCRIPT),)
ifeq ($(LAYOUT),)
LAYOUT = default
endif
ifeq ($(LAYOUT),default)
LDSCRIPT_OPTION =
else
LDSCRIPT_OPTION = --$(LAYOUT)
endif
LDSCRIPT = metal.$(LAYOUT).lds
endif

LDSCRIPT ?= metal.default.lds

LDFLAGS = -nostartfiles -T$(LDSCRIPT)

ABIFLAGS = -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL) -msave-restore

CFLAGS = -DMTIME_RATE_HZ_DEF=32768 --specs=$(RISCV_LIBC).specs -fno-common -ffunction-sections -fdata-sections $(OPT) $(ABIFLAGS) $(LDFLAGS) $(SOURCE_CFLAGS) $(FEATURE_DEFINES) -Imetal $(METAL_CFLAGS)
LIBS = $(SOURCE_LIBS)

SRC_C += $(METAL_C) $(METAL_HELPER_C)
SRC_S += $(METAL_S) $(METAL_HELPER_S)

OBJ = $(notdir $(SRC_C:.c=.o)) $(notdir $(SRC_S:.S=.o))

ifndef quiet

V?=0
# The user has explicitly enabled quiet compilation.
ifeq ($(V),0)
quiet = @printf "  $1 $2 $@\n"; $($1)
endif

# Otherwise, print the full command line.
quiet ?= $($1)

.c.o:
	@mkdir -p .deps
	$(call quiet,CC) -c $(CFLAGS) -o $@ -MT $@ -MF .deps/$*.Po -MP -MD $<

.S.o:
	@mkdir -p .deps
	$(call quiet,CC) -c $(CFLAGS) -o $@ -MT $@ -MF .deps/$*.Po -MP -MD $<
endif

$(PROGRAM): $(OBJ) $(LDSCRIPT)
	$(CC) $(CFLAGS) -o $@ $(OBJ) -Wl,-Map=$(PROGRAM).map $(LIBS)

clean::
	rm -f $(PROGRAM) $(PROGRAM).map *.o
	rm -rf metal .deps

echo::
	echo $(OBJ)

metal/metal.mk: $(METAL_MK_DEPEND)
	python3 $(FREEDOM_METAL)/scripts/codegen.py --dts $(DEVICETREE) --source-paths $(FREEDOM_METAL) $(FREEDOM_METAL)/sifive-blocks --output-dir=metal

ESDK_SETTINGS_GENERATOR ?= $(FREEDOM_METAL)/../scripts/esdk-settings-generator

metal/settings.mk: $(DEVICETREE)
	mkdir -p metal
	python3 $(ESDK_SETTINGS_GENERATOR)/generate_settings.py -t rtl -d $(DEVICETREE) -o $@

ifneq ($(LAYOUT),)

LDSCRIPT_GENERATOR ?= $(FREEDOM_METAL)/../scripts/ldscript-generator/generate_ldscript.py

$(LDSCRIPT): $(DEVICETREE)
	$(LDSCRIPT_GENERATOR) $(LDSCRIPT_OPTION) -d $(DEVICETREE) -o $@
endif

