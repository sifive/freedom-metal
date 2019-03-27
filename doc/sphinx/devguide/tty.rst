Standard I/O
============

Freedom Metal integrates with libc ``STDOUT`` and ``STDIN`` to provide virtual
terminal support. The default ``STDIO`` device is the first UART serial peripheral
on the target. If no UART serial peripheral is present, such as in the case of
SiFive CoreIP test harnesses, then the bytes sent to ``STDOUT`` are dropped and
reads of ``STDIN`` fail with error ``ENOSYS``.

Hello World
-----------

Using the virtual terminal with Freedom Metal is exactly what you might expect:

.. code-block:: C
   :linenos:

   #include <stdio.h>

   int main(void) {
      printf("Hello, world!");

      return 0;
   }

