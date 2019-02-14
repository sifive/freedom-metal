# Copyright 2018 SiFive, Inc
# SPDX-License-Identifier: Apache-2.0

# Every C compilation depends on the machine header.  This is a bit of a hack:
# instead of enforcing the dependency on the .o we pretend that the C file
# itself depends on the header.  This is the only way I could figure out that
# would satisfy the dependency inside automake.
%.c: metal/machine/@MACHINE_NAME@.h
%.S: metal/machine/@MACHINE_NAME@.h
%.o: metal/machine/@MACHINE_NAME@.h
%.c: riscv__menv__metal.specs
%.S: riscv__menv__metal.specs
%.o: riscv__menv__metal.specs
%.c: riscv__mmachine__@MACHINE_NAME@.specs
%.S: riscv__mmachine__@MACHINE_NAME@.specs
%.o: riscv__mmachine__@MACHINE_NAME@.specs

# Every test depends on the generated linker script.
$(check_PROGRAMS): riscv__mmachine__@MACHINE_NAME@.lds

# Every test depends on the freshly-compiled library.
$(check_PROGRAMS): libriscv__mmachine__@MACHINE_NAME@.a

# Generates a linker script that's more reasonable that whatever GCC's default
# is.
ldsdir = $(libdir)
lds_DATA = riscv__mmachine__@MACHINE_NAME@.lds

if PRECONFIGURED
riscv__mmachine__@MACHINE_NAME@.lds: @MACHINE_HEADER@
	cp $< $@
else # !PRECONFIGURED
riscv__mmachine__@MACHINE_NAME@.lds: @LDSCRIPT_GENERATOR@ @MACHINE_NAME@.dtb
	$< --dtb $(filter %.dtb,$^) --linker $@
endif # PRECONFIGURED

# Generates a SPEC file that sets a reasonable set of default options for this
# build.
specdir = $(libdir)
spec_DATA =

spec_DATA += riscv__mmachine__@MACHINE_NAME@.specs
if PRECONFIGURED
riscv__mmachine__@MACHINE_NAME@.specs:
	touch $@
else # !PRECONFIGURED
riscv__mmachine__@MACHINE_NAME@.specs: @SPECS_GENERATOR@ @MACHINE_NAME@.dtb
	$< --dtb $(filter %.dtb,$^) --specs $@ --prefix @prefix@ --machine @MACHINE_NAME@
endif # PRECONFIGURED

spec_DATA += riscv__menv__metal.specs
riscv__menv__metal.specs: riscv__menv__metal.specs.in
	cat $^ > $@

# In order to generate code that's actually compatible with a machine we must
# pass the march and mabi arguments to GCC that coorespond to the hardware.
# This is handled by generating a makefile fragment, including it, and then
# adding those argument to CFLAGS.
-include @MACHINE_NAME@.mk
if PRECONFIGURED
@MACHINE_NAME@.mk: @MACHINE_MAKEATTRIBUTES@
	cp $< $@
else # !PRECONFIGURED
@MACHINE_NAME@.mk: @MAKEATTRIBUTES_GENERATOR@ @MACHINE_NAME@.dtb
	$< --dtb $(filter %.dtb,$^) --output $@
endif # PRECONFIGURED

# Install some METAL-specific headers, one of which is automatically generated.
# The files that aren't automatically generated are the same for all machines.
nobase_include_HEADERS = \
	metal/machine/@MACHINE_NAME@.h \
	metal/drivers/fixed-clock.h \
	metal/drivers/riscv,clint0.h \
	metal/drivers/riscv,cpu.h \
	metal/drivers/riscv,plic0.h \
	metal/drivers/sifive,clic0.h \
	metal/drivers/sifive,fe310-g000,hfrosc.h \
	metal/drivers/sifive,fe310-g000,hfxosc.h \
	metal/drivers/sifive,fe310-g000,pll.h \
	metal/drivers/sifive,fe310-g000,prci.h \
	metal/drivers/sifive,global-external-interrupts0.h \
	metal/drivers/sifive,gpio-buttons.h \
	metal/drivers/sifive,gpio-leds.h \
	metal/drivers/sifive,gpio-switches.h \
	metal/drivers/sifive,gpio0.h \
	metal/drivers/sifive,local-external-interrupts0.h \
	metal/drivers/sifive,spi0.h \
	metal/drivers/sifive,test0.h \
	metal/drivers/sifive,uart0.h \
	metal/button.h \
	metal/clock.h \
	metal/compiler.h \
	metal/cpu.h \
	metal/interrupt.h \
	metal/io.h \
	metal/itim.h \
	metal/led.h \
	metal/machine.h \
	metal/pmp.h \
	metal/shutdown.h \
	metal/spi.h \
	metal/switch.h \
	metal/timer.h \
	metal/tty.h \
	metal/uart.h

if PRECONFIGURED
metal/machine/@MACHINE_NAME@.h: @MACHINE_HEADER@
	@mkdir -p $(dir $@)
	cp $< $@
else # !PRECONFIGURED
# Builds the machine-specific METAL header file, which paramaterizes the METAL for
# one specific machine.  This is automatically picked up by GCC so users
# automatically get the relevant definitions.  This is a two-step process:
# first a DTB is built, and then the DTB is converted to a header file using an
# external tool.
@MACHINE_NAME@.dtb: @DTC@ @MACHINE_DTS@
	$< $(filter %.dts,$^) -o $@ -O dtb -I dts

metal/machine/@MACHINE_NAME@.h: @METAL_HEADER_GENERATOR@ @MACHINE_NAME@.dtb
	@mkdir -p $(dir $@)
	$< --dtb $(filter %.dtb,$^) --output $@
endif # PRECONFIGURED

# Quash an automake warning.
lib_LIBRARIES =

# Everything in here is compiled into a single library, which contains all the
# source files in the project.  It's named for one specific machine, which GCC
# uses to select the target machine that this METAL implementation points at.
lib_LIBRARIES += libriscv__mmachine__@MACHINE_NAME@.a

libriscv__mmachine__@MACHINE_NAME@_a_CFLAGS  = @MENV_METAL@ @MMACHINE_MACHINE_NAME@
libriscv__mmachine__@MACHINE_NAME@_a_CCASFLAGS = @MENV_METAL@ @MMACHINE_MACHINE_NAME@

libriscv__mmachine__@MACHINE_NAME@_a_SOURCES = \
	src/drivers/fixed-clock.c \
	src/drivers/riscv,clint0.c \
	src/drivers/riscv,cpu.c \
	src/drivers/riscv,plic0.c \
	src/drivers/sifive,clic0.c \
	src/drivers/sifive,fe310-g000,hfrosc.c \
	src/drivers/sifive,fe310-g000,hfxosc.c \
	src/drivers/sifive,fe310-g000,pll.c \
	src/drivers/sifive,fe310-g000,prci.c \
	src/drivers/sifive,global-external-interrupts0.c \
	src/drivers/sifive,gpio-buttons.c \
	src/drivers/sifive,gpio-leds.c \
	src/drivers/sifive,gpio-switches.c \
	src/drivers/sifive,gpio0.c \
	src/drivers/sifive,local-external-interrupts0.c \
	src/drivers/sifive,spi0.c \
	src/drivers/sifive,test0.c \
	src/drivers/sifive,uart0.c \
	src/button.c \
	src/clock.c \
	src/cpu.c \
	src/entry.S \
	src/interrupt.c \
	src/led.c \
	src/pmp.c \
	src/shutdown.c \
	src/spi.c \
	src/switch.c \
	src/timer.c \
	src/tty.c \
	src/uart.c

# Freedom METAL has its own libgloss implementation that is only built in
# --with-builtin-libgloss is passed to configure.
if WITH_BUILTIN_LIBGLOSS

lib_LIBRARIES += libriscv__menv__metal.a

libriscv__menv__metal_a_SOURCES = \
	gloss/crt0.S \
	gloss/nanosleep.c \
	gloss/sys_access.c \
	gloss/sys_chdir.c \
	gloss/sys_chmod.c \
	gloss/sys_chown.c \
	gloss/sys_close.c \
	gloss/sys_execve.c \
	gloss/sys_exit.c \
	gloss/sys_faccessat.c \
	gloss/sys_fork.c \
	gloss/sys_fstat.c \
	gloss/sys_fstatat.c \
	gloss/sys_ftime.c \
	gloss/sys_getcwd.c \
	gloss/sys_getpid.c \
	gloss/sys_gettimeofday.c \
	gloss/sys_isatty.c \
	gloss/sys_kill.c \
	gloss/sys_link.c \
	gloss/sys_lseek.c \
	gloss/sys_lstat.c \
	gloss/sys_open.c \
	gloss/sys_openat.c \
	gloss/sys_read.c \
	gloss/sys_sbrk.c \
	gloss/sys_stat.c \
	gloss/sys_sysconf.c \
	gloss/sys_times.c \
	gloss/sys_unlink.c \
	gloss/sys_utime.c \
	gloss/sys_wait.c \
	gloss/sys_write.c

endif

# Quash an automake warning.
check_PROGRAMS =

# The simplest possible pair of tests: one that passes and one that fails
check_PROGRAMS       += return_pass
return_pass_SOURCES   = test/return_pass.c
return_pass_CFLAGS    = @MENV_METAL@ @MMACHINE_MACHINE_NAME@
return_pass_LDFLAGS   = -L. -Wl,--gc-sections -Wl,-Map=return_pass.map

check_PROGRAMS       += return_fail
return_fail_SOURCES   = test/return_fail.c
return_fail_CFLAGS    = @MENV_METAL@ @MMACHINE_MACHINE_NAME@
return_fail_LDFLAGS   = -L. -Wl,--gc-sections -Wl,-Map=return_fail.map

# A simple "Hello, World!" program that directly uses the METAL interface to
# print to the serial terminal.
check_PROGRAMS       += hello
hello_SOURCES         = test/hello.c
hello_CFLAGS          = @MENV_METAL@ @MMACHINE_MACHINE_NAME@
hello_LDFLAGS         = -L. -Wl,--gc-sections -Wl,-Map=hello.map

# Extra clean targets
clean-local:
	-rm -rf @MACHINE_NAME@.mk
	-rm -rf metal/machine/@MACHINE_NAME@.h @MACHINE_NAME@.dtb metal-@MACHINE_NAME@.lds
	-rm -rf *.map *.specs
