Exception Handlers
==================

CPU exceptions are the mechanism by which various execution and memory system
errors are handled. When an exception occurs, Freedom Metal will call the
corresponding exception handler function, if one has been registered by the
application.

Initializing the CPU
--------------------

When the user application enters the ``main()`` function, the Freedom
Metal framework has already set up the exception handlers.

Defining an Exception Handler
-----------------------------

Exception handlers must conform to the following function signature:

.. doxygentypedef:: metal_exception_handler_t
   :project: metal
   :no-link:

Therefore, an exception handler for the illegal instruction exception
would look like:

.. code-block:: C

   void metal_exception_illegal_instruction_handler(struct metal_cpu cpu, int ecode) {
      /* Contents of handler */
   }

Registering an Exception Handler
--------------------------------

Exception handlers are registered by name at link time. Define a
function using the appropriate name and it will be called whenever
that exception is raised. The names of the available exception
handlers are:

.. code-block:: C

   void metal_exception_instruction_address_misaligned_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_instruction_address_fault_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_illegal_instruction_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_breakpoint_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_load_address_misaligned_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_load_access_fault_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_store_amo_address_misaligned_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_store_amo_access_fault_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_ecall_from_u_mode_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_ecall_from_s_mode_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_default_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_ecall_from_m_mode_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_instruction_page_fault_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_load_page_fault_handler(struct metal_cpu cpu, int ecode);
   void metal_exception_store_amo_page_fault_handler(struct metal_cpu cpu, int ecode);


Returing Execution after a Faulting Instruction
-----------------------------------------------

The default behavior of a RISC-V CPU is to return execution to the faulting instruction.
If this is not the desired behavior, execution can be returned to the instruction after
the faulting instruction using the following method:

.. code-block:: C

   void metal_exception_load_access_fault_handler(struct metal_cpu cpu, int ecode)
   {
      /* Get the faulting instruction address */
      uintptr_t epc = metal_cpu_get_exception_pc(cpu);

      /* Get the length of the faulting instruction */
      size_t len = metal_cpu_get_instruction_length(cpu, epc);

      /* Advance stored exception program counter by the
       * instruction length */
      metal_cpu_set_exception_pc(cpu, epc + len);
   }

Additional Documentation
------------------------

Additional documentation for the exception handler API can be found in :doc:`The CPU API Reference </apiref/cpu>`.


