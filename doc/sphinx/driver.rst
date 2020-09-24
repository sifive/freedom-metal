Driver Development Guide
========================

Freedom Metal is designed to enable the creation of new device
drivers, both by adding them to the main Freedom Metal repository and
by creating separate stand-alone repositories.

Parts of a Driver
-----------------

A Freedom Metal device driver includes at least four separate files:

 1. Driver source code. This implements all of the complex driver
    logic and forms the base of the driver.

 2. Public interface header file. This defines the interface that
    applications will use in interacting with the device.

 3. Platform template. This template file uses information from Device
    Tree to construct a header file with device base address and
    register offset information.

 4. Private template. This template file also uses from Device Tree,
    but in a driver-specific fashion to build definitions for the
    driver and interrupt configuration. The resulting file is not
    expected to be useful for applications, just for the driver
    itself.

Template Files
--------------

To incorporate Device Tree data into the compiled driver, Freedom
Metal uses [Jinja](https://jinja.palletsprojects.com) templates. This
takes a C source file with special template commands that are replaced
with data computed out of the contents of the target Device Tree file.

Freedom Metal uses
[pydevicetree](https://github.com/sifive/pydevicetree/) to parse
Device Tree files and make them available to templates. Everything in
the Device Tree is available to templates.

Directory Layout
----------------

The Freedom Metal template engine expects to find files laid out in
a standard fashion:

.. graphviz::

  digraph "Freedom Metal Driver Tree" {
   rankdir=LR;
   edge [arrowsize=0.5; decorate=true; style="setlinewidth(2)"];
   node [shape=box; arrowsize=0.5; fontsize=12; height=0.2;];

   "root/" [label="Top of Driver Tree/"];
   "src_drivers/" [label="drivers/"];
   "metal_drivers/" [label="drivers/"];
   "templates_metal/" [label="metal/"];
   "templates_metal_platform/" [label="platform/"];
   "templates_metal_private/" [label="private/"];

   "root/" -> "src/" [weight=1000];
   "root/" -> "metal/";
   "root/" -> "templates/";
   "src/" -> "src_drivers/";
   "src_drivers/" -> "<driver>.c";
   "metal/" -> "metal_drivers/"
   "metal_drivers/" -> "<driver>.h";
   "templates/" -> "MANIFEST.ini";
   "templates/" -> "templates_metal/";
   "templates_metal/" -> "templates_metal_platform/";
   "templates_metal/" -> "templates_metal_private/";
   "templates_metal_platform/" -> "metal_platform_<driver>.h.j2";
   "templates_metal_private/" -> "metal_private_<driver>.h.j2";
  }


Exposing Devices use Freedom Metal APIs.
----------------------------------------

Each device driver in Freedom Metal creates a unique driver-specific
interface. This effectively provides separate name spaces for each
driver and avoids symbol collisions. Applications may use these
driver-specific interfaces if desired, but more general
purpose code can target a broader range of hardware by using the
generic Freedom Metal interfaces. For example, the SiFive UART driver
exposes an interface which is prefixed with ``sifive_uart0_``. A set of
those functions is also compatible with the generic
Freedom Metal uart device, and so there is an entry in
``sifive_blocks/templates/MANIFEST.ini``:

.. code-block:: none

   [uart]
   Compatible = sifive,uart0

With this declaration, freedom metal creates aliases which map the
generic UART API to the SiFive UART driver, allowing applications to
use those names.

Templates
---------

A key part of this system is the use of templates to build
target-specific code using Device Tree and application definitions.  A
more complete description of the Jinja template language can be
found on the [Jinja website](https://jinja.palletsprojects.com).

Let's look at the template which generates the Platform header file
for the SiFive UART, which is found in
``sifive-blocks/templates/metal/platform/metal_platform_sifive_uart0.h.j2``:

.. code-block:: c

    #ifndef METAL__PLATFORM__SIFIVE_UART0_H
    #define METAL__PLATFORM__SIFIVE_UART0_H

    {% if 'sifive,uart0' in devices %}
    {% set sifive_uart0s = devices['sifive,uart0'] %}

    {% for uart in sifive_uart0s %}
    {% if uart.reg_names is defined %}
    #define METAL_SIFIVE_UART0_{{ loop.index0 }}_BASE_ADDRESS {{ '0x%x' % uart.regs_by_name["control"][0] }}
    #define METAL_SIFIVE_UART0_{{ loop.index0 }}_SIZE {{ '0x%x' % uart.regs_by_name["control"][1] }}
    {% else %}
    #define METAL_SIFIVE_UART0_{{ loop.index0 }}_BASE_ADDRESS {{ '0x%x' % uart.reg[0][0] }}
    #define METAL_SIFIVE_UART0_{{ loop.index0 }}_SIZE {{ '0x%x' % uart.reg[0][1] }}
    {% endif %}
    {% endfor %}

    #define METAL_SIFIVE_UART0
    #define METAL_SIFIVE_UART0_TXDATA 0UL
    #define METAL_SIFIVE_UART0_RXDATA 4UL
    #define METAL_SIFIVE_UART0_TXCTRL 8UL
    #define METAL_SIFIVE_UART0_RXCTRL 12UL
    #define METAL_SIFIVE_UART0_IE 16UL
    #define METAL_SIFIVE_UART0_IP 20UL
    #define METAL_SIFIVE_UART0_DIV 24UL

    {% endif %}

    #endif

This header file defines constants needed to address the UART device
registers, using the data found in the UART device tree node:

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

Combining these two together, the template system generates the output
file ``metal/platform/metal_platform_sifive_uart0.h``:

.. code-block:: c

    #ifndef METAL__PLATFORM__SIFIVE_UART0_H
    #define METAL__PLATFORM__SIFIVE_UART0_H

    #define METAL_SIFIVE_UART0_0_BASE_ADDRESS 0x10013000
    #define METAL_SIFIVE_UART0_0_SIZE 0x1000

    #define METAL_SIFIVE_UART0
    #define METAL_SIFIVE_UART0_TXDATA 0UL
    #define METAL_SIFIVE_UART0_RXDATA 4UL
    #define METAL_SIFIVE_UART0_TXCTRL 8UL
    #define METAL_SIFIVE_UART0_RXCTRL 12UL
    #define METAL_SIFIVE_UART0_IE 16UL
    #define METAL_SIFIVE_UART0_IP 20UL
    #define METAL_SIFIVE_UART0_DIV 24UL

    #endif

The Platform header file must be named
``metal_platform_sifive_uart0.h``, as the template system uses the
``compatible`` value ``"sifive,uart0"`` to generate that file
name. Elements within that Device Tree node are then available to the
template using the names found in template, with all punctuation
mapped to ``_``. There are a few ways where the template system makes
writing templates a bit easier:

 * If there is a ``reg-names`` entry in the device-tree node, then those
   names may be used as keys in the ``regs_by_name`` dictionary in place
   of indexes into the ``reg`` array (as shown in the example above).

 * If there is a ``clock-names`` entry in the device-tree node, then
   those names may be used as keys in the ``clocks_by_name`` dictionary in place
   of indexes into the ``clocks`` array. The example above does not
   provide a name for the clock.

 * If the ``reg`` entry uses references to other device tree nodes,
   (e.g. ``reg = <&aon 0x70 &aon 0x73>;``), then those references are
   resolved and added to the offset values. This usage is
   non-standard; all drivers should instead directly refer to the
   referenced block which should define the relevant register offsets.

 * Device Tree references in values are converted into python
   references for use by your template.

 * ``id`` numbers are assigned for each node. These index instances of
   hardware using the same driver. When there is only one instance of
   a particular kind of hardware, the index for that instance will be
   zero.

Here's an example from the hfxosc driver of how the references can be
used. Let's look at the device tree node for that:

.. code-block:: none

    hfxoscout: clock@1 {
	    compatible = "sifive,fe310-g000,hfxosc";
	    clocks = <&hfxoscin>;
	    reg = <&prci 0x4>;
	    reg-names = "config";
    };

In this case, the driver needs to be able to find the referenced clock
``&hfxoscin``. When there's only one hfxoscout node, the private
template fragment looks like:

.. code-block:: c

    #define REF_CLOCK(clock) ((struct metal_clock) { {{ sifive_fe310_g000_hfxoscs[0].clocks[0].id }} })

This looks up the first clock in the device, fetches the first clock
node referenced by that and returns it's unique identifier, which is
then used to construct a metal_clock struct containing that ID. This
value can be directly used with the generic ``metal_clock`` API as
defined by ``metal_clock.h``.

Platform Template File Contents
-------------------------------

The Platform Template file should be named
``templates/metal/platform/metal_platform``*compatible*``.h.j2``. If the compatible string in
Device Tree is ``sifive,fe310-g000,hfxosc``, then the platform template
file name would be
``templates/metal/platform/metal_platform_fe310_g000_hfxosc.h.j2``.  This
header file is exposed to applications and so it should only define
names which will not conflict with names selected by an
application. The standard of practice here is to prefix all names with
``METAL_``*COMPATIBLE* to ensure uniqueness.

This file should define the address and size of any register block for
the device along with register offsets within that device. You can see
an example of this file for ``sifive,uart0`` above.

Private Template File Contents
------------------------------

The Private Template file should be named
``templates/metal/private/metal_private``*compatible*``.h.j2``. If the
compatible string in Device Tree is ``sifive,fe310-g000,hfxosc``, then
the private template file name would be
``templates/metal/private/metal_private_fe310_g000_hfxosc.h.j2``.
This file contains data for use by the freedom-metal driver. It should
contain definitions for static data needed by the driver which can be
derived from the Device Tree file, including references to other
devices in the system for clocking or pin configuration. Where
possible, the generic freedom-metal API for those devices should be
used to make the driver as portable as possible to other SoC
configurations.

The Private Template file is also the place where clock and interrupt
dispatch definitions are specified. These configure the referenced
clock and interrupt controllers for our device. Let's walk through the
``sifive,uart0`` Private Template.

.. code-block:: C

    #ifndef METAL__DRIVERS__SIFIVE_UART0_H
    #define METAL__DRIVERS__SIFIVE_UART0_H

    {% include 'template_comment.h.j2' %}

    #include <metal/clock.h>
    #include <metal/private/metal_private_uart.h>
    #include <metal/gpio.h>
    #include <metal/interrupt.h>
    #include <metal/platform.h>
    #include <stdbool.h>
    #include <stddef.h>

This portion wraps the header file in a guard #ifdef to prevent the
contents from being parsed more than once by the compiler, then
includes all of the necessary header files

.. code-block:: C

    {% if 'sifive,uart0' in devices %}

This statement elides the rest of the contents of file if there are no
``sifive,uart0`` compatible devices in the target system.

.. code-block:: C

    {% set sifive_uart0s = devices['sifive,uart0'] %}

This defines a temporary variable to hold the list of ``sifive,uart0``
devices in the system. Doing this makes the rest of the code much
easier to understand.

The next section defines a constant data structure to hold all of the
mappings between the ``sifive,uart0`` device and other devices in the
system. Here's the template for that,
``templates/metal/private/metal_private_sifive_uart0.h.j2``:

.. code-block:: C

    static const struct dt_uart_data {
	    uintptr_t base_addr;
	    struct metal_clock clock;
	    bool has_pinmux;
	    struct metal_gpio pinmux;
	    uint32_t pinmux_output_selector;
	    uint32_t pinmux_source_selector;
	    struct metal_interrupt interrupt_parent;
	    uint32_t interrupt_id;
    } dt_uart_data[__METAL_DT_NUM_UARTS] = {
	    {% for uart in sifive_uart0s %}
	    {
		.base_addr = METAL_SIFIVE_UART0_{{ uart.id }}_BASE_ADDRESS,

	    {% if uart.clocks is defined %}
		/* {{ uart.clocks[0].compatible[0] }} */
		.clock = { {{ uart.clocks[0].id }} },
	    {% endif %}

	    {% if uart.pinmux is defined %}
		/* {{ uart.pinmux[0].compatible[0] }} */
		    .has_pinmux = 1,
		    .pinmux = { {{ uart.pinmux[0].id }} },
		    .pinmux_output_selector = {{ "0x%x" % uart.pinmux[1] }},
		    .pinmux_source_selector = {{ "0x%x" % uart.pinmux[2] }},
	    {% else %}
		    .has_pinmux = 0,
	    {% endif %}

	    {% if uart.interrupt_parent is defined %}
		/* {{ uart.interrupt_parent[0].compatible[0] }} */
		    .interrupt_parent = { {{ uart.interrupt_parent[0].id }} },
		    .interrupt_id = {{ uart.interrupts[0] }},
	    {% endif %}
	    },
	    {% endfor %}
    };

Here's all of the Device Tree data relevant to the ``sifive,uart0``
driver, including the referenced clocks, the pinmux configuration and
the interrupt controller. This uses the
``METAL_SIFIVE_UART0_``*id*``BASE_ADDRESS definition from the Platform
Template file along with the ``pinmux`` and ``interrupt-parent``
referenced devices.

The next section maps the specific devices used for clocking and
interrupts to the generic ``metal_clock`` and ``metal_interrupt`` APIs
so that the ``sifive,uart0`` driver can be written using those
interfaces and get compiled to directly call the relevant drivers:

.. code-block:: c

    {% if sifive_uart0s[0].clocks is defined %}
    {% set driver_string = to_snakecase(sifive_uart0s[0].clocks[0].compatible[0]) %}
    {% include 'clock_dispatch.h.j2' %}
    {% endif %}

    {% if sifive_uart0s[0].interrupt_parent is defined %}
    {% set driver_string = to_snakecase(sifive_uart0s[0].interrupt_parent[0].compatible[0]) %}
    {% include 'interrupt_dispatch.h.j2' %}
    {% endif %}

These fragments pull out the compatible strings from the referenced
devices and construct driver names using the built-in ``to_snakecase``
function. The computed ``driver_string`` values are used within those
templates to map the generic API to driver-specific names.

The last part of the ``sifive,uart0`` Private Template uses the
``chosen`` node from Device Tree to direct output to ``stdout`` to the
right uart. Here's the ``chosen`` node from the SiFive HiFive 1 Rev B
board:

.. code-block:: none

    chosen {
	    metal,entry = <&spi0 1 65536>;
	    metal,boothart = <&L6>;
	    stdout-path = "/soc/serial@10013000:115200";
	    metal,itim = <&itim 0 0>;
	    metal,ram = <&dtim 0 0>;
    };

The relevant line here is the ``stdout-path`` entry, which the
Template Generator automatically converts into an array containing a
reference to the named Device Tree node and a baud rate for use by the
template:

.. code-block:: c

    {% if chosen.stdout_path is defined %}
    {% if chosen.stdout_path[0].compatible[0] == "sifive,uart0" %}

    /* sifive,uart0 has been selected by the Devicetree to provide
     * standard out for the Freedom Metal program
     */
    #define METAL_STDOUT_SIFIVE_UART0
    #define __METAL_DT_STDOUT_UART_HANDLE ((struct metal_uart) { {{ chosen.stdout_path[0].id }} })
    #define __METAL_DT_STDOUT_UART_BAUD {{ chosen.stdout_path[1] }}
    {% endif %}
    {% endif %}

This first checks to see if the ``stdout-path`` value references a
``sifive,uart0`` device, then pulls the ``id`` field from that device
to hook up the correct device and baud rate.

On the HiFive1 Revb board, this generates the
following ``metal/private/metal_private_sifive_uart0.h`` file:

.. code-block:: c
    #ifndef METAL__DRIVERS__SIFIVE_UART0_H
    #define METAL__DRIVERS__SIFIVE_UART0_H

    /* GENERATED FILE
     * --------------
     * This file is generated from a template based on the content of the target
     * Devicetree.
     */
    #include <metal/clock.h>
    #include <metal/private/metal_private_uart.h>
    #include <metal/gpio.h>
    #include <metal/interrupt.h>
    #include <metal/platform.h>
    #include <stdbool.h>
    #include <stddef.h>


    static const struct dt_uart_data {
	    uintptr_t base_addr;
	    struct metal_clock clock;
	    bool has_pinmux;
	    struct metal_gpio pinmux;
	    uint32_t pinmux_output_selector;
	    uint32_t pinmux_source_selector;
	    struct metal_interrupt interrupt_parent;
	    uint32_t interrupt_id;
    } dt_uart_data[__METAL_DT_NUM_UARTS] = {
	    {
		.base_addr = METAL_SIFIVE_UART0_0_BASE_ADDRESS,

		/* sifive,fe310-g000,pll */
		.clock = { 0 },

		/* sifive,gpio0 */
		    .has_pinmux = 1,
		    .pinmux = { 0 },
		    .pinmux_output_selector = 0x0,
		    .pinmux_source_selector = 0x30000,

		/* riscv,plic0 */
		    .interrupt_parent = { 0 },
		    .interrupt_id = 5,
	    },
    };


    #include <metal/drivers/sifive_fe310_g000_pll.h>

    /* These defines "redirect" the calls to the public Freedom Metal clock API
     * to the driver for the device at compile time. Since they are the same
     * as the actual public API symbols, when they aren't defined (for instance,
     * if the Devicetree doesn't properly describe what clock drives a device)
     * they will link to the stub functions in src/clock.c
     */

    #define metal_clock_get_rate_hz(clock) sifive_fe310_g000_pll_get_rate_hz((clock))
    #define metal_clock_set_rate_hz(clock, rate) sifive_fe310_g000_pll_set_rate_hz((clock), (rate))

    #include <metal/drivers/riscv_plic0.h>

    /* These defines "redirect" the calls to the public Freedom Metal interrupt API
     * to the driver for the controller at compile time. Since they are the same
     * as the actual public API symbols, when they aren't defined (for instance,
     * if the Devicetree doesn't properly describe the interrupt parent for the device)
     * they will link to the stub functions in src/interrupt.c
     */

    #define metal_interrupt_init(intc) riscv_plic0_init((intc))
    #define metal_interrupt_enable(intc, id) riscv_plic0_enable((intc), (id))
    #define metal_interrupt_disable(intc, id) riscv_plic0_disable((intc), (id))

    /* sifive,uart0 has been selected by the Devicetree to provide
     * standard out for the Freedom Metal program
     */
    #define METAL_STDOUT_SIFIVE_UART0
    #define __METAL_DT_STDOUT_UART_HANDLE ((struct metal_uart) { 0 })
    #define __METAL_DT_STDOUT_UART_BAUD 115200


    #endif

Metal Driver Header
-------------------

This is an optional file where you can place fixed definitions for
your driver and is named ``metal/drivers/``*compatible*``.h``. If the
compatible string in Device Tree is ``sifive,fe310-g000,hfxosc``, then
the metal driver header file name would be
``metal/drivers/sifive_fe310_g000_hfxosc.h``.  It is a public header
file for interfaces which are not part of the generic Freedom Metal
API for your device. As the ``sifive,uart0`` driver does not declare
any interfaces beyond the generic ``metal_uart`` interface, it does
not include this header file.

Metal Driver Source
-------------------

The device driver source code file name is
``metal/src/drivers/``*compatible*``.c. If the compatible string in
Device Tree is ``sifive,fe310-g000,hfxosc``, then the metal driver
source file name would be
``metal/src/drivers/sifive_fe310_g000_hfxosc.c``.

Handling Interrupts
-------------------

Drivers that can generate interrupts should expose interrupt enable
and disable functions so that applications can manage device
interrupts without needing to understand the system interrupt
connectivity. Device Tree provides that information for the driver.

Here's an example of _enable and _disable functions for
``sifive,uart0``:

.. code-block:: c

    static __inline__ int enable_parent_interrupt(struct metal_uart uart) {
	struct metal_interrupt intc =
	    dt_uart_data[get_index(uart)].interrupt_parent;
	int id = dt_uart_data[get_index(uart)].interrupt_id;

	return metal_interrupt_enable(intc, id);
    }

    static __inline__ int disable_parent_interrupt(struct metal_uart uart) {
	struct metal_interrupt intc =
	    dt_uart_data[get_index(uart)].interrupt_parent;
	int id = dt_uart_data[get_index(uart)].interrupt_id;

	return metal_interrupt_disable(intc, id);
    }

    int sifive_uart0_tx_interrupt_enable(struct metal_uart uart) {
	uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

	UART_REGW(METAL_SIFIVE_UART0_IE) |= UART_TXWM;

	return enable_parent_interrupt(uart);
    }

    int sifive_uart0_tx_interrupt_disable(struct metal_uart uart) {
	uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

	UART_REGW(METAL_SIFIVE_UART0_IE) &= ~UART_TXWM;

	if ((UART_REGW(METAL_SIFIVE_UART0_IE) & UART_RXWM) == 0) {
	    /* Disable the UART interrupt line on the interrupt controller
	     * when no UART interrupt sources are enabled */
	    return disable_parent_interrupt(uart);
	}
	return 0;
    }

    int sifive_uart0_rx_interrupt_enable(struct metal_uart uart) {
	uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

	UART_REGW(METAL_SIFIVE_UART0_IE) |= UART_RXWM;

	return enable_parent_interrupt(uart);
    }

    int sifive_uart0_rx_interrupt_disable(struct metal_uart uart) {
	uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

	UART_REGW(METAL_SIFIVE_UART0_IE) &= ~UART_RXWM;

	if ((UART_REGW(METAL_SIFIVE_UART0_IE) & UART_TXWM) == 0) {
	    /* Disable the UART interrupt line on the interrupt controller
	     * when no UART interrupt sources are enabled */
	    return disable_parent_interrupt(uart);
	}

	return 0;
    }


This code enables interrupts in the UART device and calls the parent
interrupt controller code to manage the interrupt there. If there are
further interrupt controllers in the hierarchy, then the parent
interrupt controller code would be responsible for calling that
driver.

You can see this code using the ``dt_uart_data`` structure that was
defined in ``templates/metal/private/metal_private_sifive_uart.h.j2``
including the ``interrupt_parent`` and ``interrupt_id`` values. These
functions are using names that will get mapped to the generic UART API
because the ``templates/MANIFEST.ini`` file declares the
``sifive,uart0`` driver as being compatible with the uart
requirements.

Handling Clocking
-----------------

As clock rates can change while the system is running, Freedom Metal
provides a mechanism for drivers to register callbacks that are
invoked before and after the clock rate changes. The ``sifive,uart0``
driver provides these callbacks to compute the right register values
based on the provided clock and specified baud rate:

.. code-block:: c

   void _sifive_uart0_pre_rate_change_callback(uint32_t id);

   void _sifive_uart0_post_rate_change_callback(uint32_t id);
		
The private template for clock drivers which may change the frequency
it supplies should include a stanza which generates calls to these
functions. For instance, in the ``sifive,fe310-g000,pll`` private
header template, you'll find:

.. code-block:: c

    {% for compat in devices %}
	{% if 'clocks' in devices[compat][0] %}
	    {% if devices[compat][0].clocks[0].compatible[0] == "sifive,fe310-g000,pll" %}
    void _{{ to_snakecase(devices[compat][0].compatible[0]) }}_pre_rate_change_callback(uint32_t id);
    void _{{ to_snakecase(devices[compat][0].compatible[0]) }}_post_rate_change_callback(uint32_t id);
	    {% endif %}
	{% endif %}
    {% endfor %}

    static __inline__ void pre_rate_change_callbacks(void) {
    {% if sifive_fe310_g000_plls|length == 1 %}
	{% for compat in devices %}
	    {% for device in devices[compat] %}
		{% if 'clocks' in device %}
		    {% if device.clocks[0].compatible[0] == "sifive,fe310-g000,pll" %}
		_{{ to_snakecase(device.compatible[0]) }}_pre_rate_change_callback({{ device.id }});
		    {% endif %}
		{% endif %}
	    {% endfor %}
	{% endfor %}
    {% endif %}
    }

    static __inline__ void post_rate_change_callbacks(void) {
    {% if sifive_fe310_g000_plls|length == 1 %}
	{% for compat in devices %}
	    {% for device in devices[compat] %}
		{% if 'clocks' in device %}
		    {% if device.clocks[0].compatible[0] == "sifive,fe310-g000,pll" %}
		_{{ to_snakecase(device.compatible[0]) }}_post_rate_change_callback({{ device.id }});
		    {% endif %}
		{% endif %}
	    {% endfor %}
	{% endfor %}
    {% endif %}
    }

If the UART is connected to a fixed-frequency clock, these functions
will never be referenced and the linker will elide them from the final
object when the ``-Wl,--gc-sections`` argument is used to build it.

Device Initialization
---------------------

The driver init function is called by the application before it starts
using the device. This function needs to configure the device for
operation and make sure whatever signals it uses are routed to the
right pins on the package.

For devices with GPIO pin multiplexing, setting up the pin routing is
done by calling ``metal_gpio_enable_pinmux`` to set that up. To do that, the
private template header file checks the Device Tree configuration to
sets the ``has_pinmux`` value when there is one and then also sets the
``pinmux_output_selector`` and ``pinmux_source_selector`` values. In
the init code, those are used as follows:

.. code-block:: c

    void sifive_uart0_init(struct metal_uart uart, uint32_t baud_rate) {
	uint32_t index = get_index(uart);

	sifive_uart0_set_baud_rate(uart, baud_rate);

	int has_pinmux = dt_uart_data[index].has_pinmux;
	if (has_pinmux) {
	    struct metal_gpio pinmux = dt_uart_data[index].pinmux;
	    uint32_t output_sel = dt_uart_data[index].pinmux_output_selector;
	    uint32_t source_sel = dt_uart_data[index].pinmux_source_selector;
	    metal_gpio_enable_pinmux(pinmux, output_sel, source_sel);
	}
    }

