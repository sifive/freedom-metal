Interrupt Handlers
==================

Interrupt Handlers
------------------

Interrupt handlers must conform to the following function signature:

.. doxygentypedef:: metal_interrupt_handler_t
   :project: metal

Therefore, an interrupt handler for a UART might look like:

.. code-block:: C

   void metal_sifive_uart0_source_0_handler(void) {
      /* Contents of handler */
   }

Registering an Interrupt Handler
--------------------------------

Freedom Metal interrupt controllers define a vector of interrupts for
devices connected through them using information found in the Device
Tree file. This means that an interrupt handler is registered with the
controller when it is declared using the correct name.

Additional Documentation
------------------------

Additional documentation for the interrupt handler API can be found in
:doc:`the CPU API reference </apiref/cpu>` and
:doc:`the Interrupt API reference </apiref/interrupt>`.

