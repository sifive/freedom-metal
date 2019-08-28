Constructors and Destructors
============================

Metal implements a mechanism for registering constructors and destructors to
run before main and upon exit. The purpose of the Metal init API is to provide
a unified mechanism for registering constructors for both users and drivers,
as well as to provide a mechanism for manually overriding or disabling the
constructors/destructors entirely.

Defining a Constructor/Destructor
---------------------------------

Constructors and destructors can be defined with the following macros:

.. doxygendefine:: METAL_CONSTRUCTOR
   :project: metal

.. doxygendefine:: METAL_CONSTRUCTOR_PRIO
   :project: metal

.. doxygendefine:: METAL_DESTRUCTOR
   :project: metal

.. doxygendefine:: METAL_DESTRUCTOR_PRIO
   :project: metal

For example:

.. code-block:: C

   METAL_CONSTRUCTOR(constructor_hello) {
      puts("Hello from before main!\n");
   }

   METAL_DESTRUCTOR_PRIO(destructor_goodbye, METAL_INIT_HIGHEST_PRIORITY) {
      puts("Program exiting, goodbye.\n");
   }

The above sample defines the functions ``constructor_hello()`` and
``constructor_goodbye()`` and registers them to be run by ``metal_init()`` and
``metal_fini()``.

.. doxygenfunction:: metal_init
   :project: metal

.. doxygenfunction:: metal_fini
   :project: metal

Default Behavior
----------------

By default, Metal constructors and destructors are run before main and upon exit
respectively. This ensures that constructors defined by Metal and
Metal device drivers are called by default before ``main()``. For example, targets
with the "sifive,uart0" UART device set as ``stdout-path`` automatically configure
the UART's clock divider to the requested baud rate using a Metal constructor.

The default control flow looks like the following:

 * _start

   * ...
   * metal_init_run

     * metal_init

       * constructor_1
       * constructor_2
       * ...

   * ...
   * main

     * ...

   * exit

     * ...
     * metal_fini_run

       * metal_fini

         * destructor_1
         * destructor_2
         * ...

     * ...

Note ``metal_init_run()`` and ``metal_fini_run()`` in the above flow graph.

.. doxygenfunction:: metal_init_run
   :project: metal

.. doxygenfunction:: metal_fini_run
   :project: metal

The purpose of these wrapper functions is to allow manual override by application
code.

Preventing Constructors/Destructors from Running
------------------------------------------------

You can prevent Metal constructors and destructors from running by redifining
``metal_init_run()`` and ``metal_fini_run()`` in your application:

.. code-block:: C

   void metal_init_run() {}
   void metal_fini_run() {}
