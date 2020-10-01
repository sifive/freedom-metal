**********************
Freedom Metal Generate
**********************

To incorporate Device Tree data into the compiled driver, Freedom
Metal includes **freedom-metal-generate**, a Python program which uses
Jinja_ templates to create target- and application- specific source
files to control compilation of Freedom Metal
drivers. Freedom-metal-generate takes source files which mix C code
with special template commands that are replaced with data computed
out of the contents of the target Device Tree file and application
configuration file.

Freedom-metal-generate uses pydevicetree_ to parse Device Tree files
and make them available to templates. Everything in the Device Tree is
available to templates.

.. _Jinja: https://jinja.palletsprojects.com
.. _pydevicetree: https://github.com/sifive/pydevicetree/

Running freedom-metal-generate
------------------------------

Freedom-metal-generate is designed to be run from within your
application's build system. It uses both target-device-specific
information from Device Tree as well as application-specific
information from a configuration file.

.. code-block:: none

    $ FREEDOM_METAL=/path/to/freedom-metal
    $ python3 $(FREEDOM_METAL)/scripts/freedom-metal-generate \
	--dts design.dts \
	--source-paths $(FREEDOM_METAL) $(FREEDOM_METAL)/sifive-blocks \
	--output-dir . \
        --application-config application.ini
    
Freedom-metal-generate options:

 * ``--dts``. Specifies a Device Tree source file that describes the
   target device, including all devices for which the application
   wants to use Freedom Metal drivers.

 * ``--source-paths``. Specifies a list of directories that fit the
   Freedom Metal directory layout discussed next. Freedom metal
   drivers can be placed in any directory which follows these
   conventions and included in the build by listing them here. Note
   that the Freedom Metal code is split into two such paths; the first
   includes a generic RISC-V drivers while the second contains drivers
   for SiFive peripherals.

 * ``--output-dir``. Specifies the directory to place the generated
   files in.

 * ``--application-config``. Specifies a file which controls
   how the generated files configure the hardware.
   
Interrupt Configuration
-----------------------

RISC-V interrupt hardware offers sophisticated mechanisms to fine-tune
interrupt performance and behavior in your application. Applications
can lower latency by creating interrupt handlers that perform less
setup and tear-down. Alternatively, they can allow handlers to be
preempted by higher priority interrupts by spending a bit more time
setting things up at interrupt entry time.

The Freedom Metal Application Configuration file provides settings for
overall interrupt mode, individual interrupt priority and individual
interrupts vectoring mode.

The file consists of two major sections, the ``[interrupts.priority]``
section, which sets hardware priority values for individual
interrupts, and then ``[interrupts.hwvector]`` section, which changes
how the interrupt drivers manage individual interrupts.

Interrupt priorities are set for each interrupt controller by
specifying the ``compatible`` name of the device which the interrupt
is connected to and the index of the interrupt in that devices
``interrupts`` entry.

For instance, to set the priority of the UART interrupt as
specified by this Device Tree fragment:

.. code-block:: none

    uart0: serial@10013000 {
	    compatible = "sifive,uart0";
	    interrupt-parent = <&plic>;
	    interrupts = <3>;
	    reg = <0x10013000 0x1000>;
	    reg-names = "control";
	    clocks = <&hfclk>;
	    pinmux = <&gpio0 0x0 0x30000>;
    };

The application configuration file would contain an
``[interrupts.priority]`` section that looks like this:

.. code-block:: none

    [interrupts.priority]
    sifive,uart0 = 0:4

This sets the first interrupt configured for the interrupt ``<3>`` to
priority 4. If there are multiple interrupts needing configuration,
separate them with spaces. For example, a ``sifive,pwm0`` device with
a Device Tree fragment like:

.. code-block:: none

    L8: pwm@20005000 {
	    clocks = <&L0>;
	    compatible = "sifive,pwm0";
	    interrupt-parent = <&L1>;
	    interrupts = <65 66 67 68>;
	    reg = <0x20005000 0x1000>;
	    reg-names = "control";
	    sifive,comparator-widthbits = <8>;
	    sifive,ncomparators = <4>;
    };
   
could have its interrupt priorities adjusted like:

.. code-block:: none

   [interrupts.priority]
   sifive,pwm0 = 0:4 1:3 2:5 3:6

The other section in the application configuration file related to
interrupts adjusts how the interrupt vectors are invoked. By default,
device interrupt vectors are called from the interrupt controller
driver instead of being directly wired to the hardware. This allows
interrupt controller to manage interrupt preemption and chaining
while allowing device interrupt vectors to be written with a C ABI
instead of a low-level ABI.

Using software vectoring like this is generally a good idea for
improving overall performance of the system. For highly
latency-sensitive hardware, the extra steps needed may be more than
they can tolerate, and for those, Freedom Metal allows them to be
directly wired to the interrupt hardware vector address.

Using our ``sifive,uart0`` example from above, here's a application
configuration file fragment which makes the interrupt hardware
vectored:

.. code-block:: none

    [interrupts.hwvector]
    sifive,uart0 = 0

This does two things. First, it declares the vector with
``__attribute__((interrupt))`` so that it will be compiled with an
interrupt ABI instead of a C ABI. This saves and restores registers
correctly for this mode:

.. code-block:: C

    void metal_sifive_uart0_source_0_handler(void) __attribute__((interrupt));

Second, it causes that interrupt to be configured as HW-vectored in
the interrupt controller initialization routine:

.. code-block:: C

    static void clic_configure_vectoring(void) {
	/* Software-vectored interrupts are enabled. Configure any selectively-vectored
	 * interrupts now. */
	struct metal_interrupt clic = (struct metal_interrupt){0};
	sifive_clic0_vector_enable(clic, 64);
    }

Build System Integration
------------------------

Freedom-metal-generate also creates a Makefile fragment that can be
used when building your application. This file defines a set of
Make variables that reference the Freedom Metal sources.

 * ``METAL_SRC``. This is a list of all Freedom Metal C and assembly
   source files. All of these files should be compiled and linked into
   your application.

 * ``METAL_CFLAGS``. This lists the include directives needed to find
   the header files while compiling your application. They should be
   included in the compiler command line.

 * ``METAL_MK_DEPEND``. This lists all of the files upon which the
   files generated by freedom-metal-generate depend. You can use this
   to automatically re-run freedom-metal-generate when its output may
   change.

 * ``METAL_SRC_PATH``. This lists all of the directories (separated by
   ':') which contain Freedom Metal C and assembly source code. You
   can use this in a VPATH Make directive.

To generate fragments for other build systems, create a suitable
template in one of the source paths using the existing ``metal.mk.j2``
template as an example.
