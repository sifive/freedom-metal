Constructors
============

Metal implements a mechanism for registering constructors to run
before main. The purpose of the Metal init API is to provide a unified
mechanism for registering constructors for both users and drivers, as
well as to provide a mechanism for manually overriding or disabling
the constructors entirely.

Defining a Constructor
---------------------------------

Constructors can be defined with the following macros:

.. doxygendefine:: METAL_CONSTRUCTOR
   :project: metal

.. doxygendefine:: METAL_CONSTRUCTOR_PRIO
   :project: metal

For example:

.. code-block:: C

   METAL_CONSTRUCTOR(constructor_hello) {
      puts("Hello from before main!\n");
   }

   METAL_CONSTRUCTOR_PRIO(constructor_hello_early, METAL_INIT_HIGHEST_PRIORITY) {
      puts("Hello from earliest constructor.\n");
   }

The above sample defines the functions ``constructor_hello()`` and
``constructor_hello_early()`` and registers them to be run by ``metal_init()``.

.. doxygenfunction:: metal_init
   :project: metal

Default Behavior
----------------

By default, Metal constructors are run before main. This ensures that
constructors defined by Metal and Metal device drivers are called by
default before ``main()``. For example, targets with the
"sifive,uart0" UART device set as ``stdout-path`` automatically
configure the UART's clock divider to the requested baud rate using a
Metal constructor.

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

Note ``metal_init_run()`` in the above flow graph.

.. doxygenfunction:: metal_init_run
   :project: metal

The purpose of these wrapper functions is to allow manual override by application
code.

Preventing Constructors from Running
------------------------------------------------

You can prevent Metal constructors from running by redefining
``metal_init_run()`` in your application:

.. code-block:: C

   void metal_init_run() {}
