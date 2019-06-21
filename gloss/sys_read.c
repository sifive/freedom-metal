#include <metal/tty.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

/* Known Bugs:
 * ======
 * The content of tty_buffer has to be filled before calling
 * metal_tty_putc(). This is because metal_tty_putc() always
 * modifies the content of the tty_buffer array, though this
 * array is declared static in this file scope. 
 *
 * This happens after
 * inline int metal_uart_putc(struct metal_uart *uart, int c) { 
 *     return uart->vtable->putc(uart, c);
 * }
 * is called in metal/uart.h.
 * ======
 * When TTY_BUFFER_LEN is set to a small number (such as 10),
 * metal_tty_putc() will again modify the values of start_index
 * and end_index. 
 *
 * This happens after 
 * __metal_driver_sifive_uart0_txready(uart)
 * is called in src/drivers/sifive_uart0.c. */

/* Parameters for the circular input buffer */
#define TTY_BUFFER_LEN (127)
#define WRAP_LEN (TTY_BUFFER_LEN + 1)

static char tty_buffer[WRAP_LEN];
static uint8_t start_index;
static uint8_t end_index;
#define INCREASE_INDEX(n) (n = (n + 1) % WRAP_LEN)
#define DECREASE_INDEX(n) (n = (n + TTY_BUFFER_LEN) % WRAP_LEN)
#define BUFFER_LEN (end_index > start_index ? \
                    end_index - start_index : \
                    end_index + WRAP_LEN - start_index)
/* This macro backs up start_index and end_index
 * on the stack. This is only a temporary solution to the 
 * bugs described above */
#define BACKUP_INDICES(statement) \
    start_backup = start_index; \
    end_backup = end_index; \
    statement \
    start_index = start_backup; \
    end_index = end_backup;

static void present_buffer(void) {
  char c;
  int int_buffer;
  uint8_t start_backup;
  uint8_t end_backup;
  while (1) {
    metal_tty_getc(&int_buffer);
    /* If input is nothing, do nothing */
    if (int_buffer == -1) {
      continue;
    }
    c = int_buffer;
    /* Prevent overflowing the buffer */
    if (BUFFER_LEN == TTY_BUFFER_LEN) {
        if (c == 010 || c == 0177) {
            DECREASE_INDEX(end_index);
            BACKUP_INDICES(metal_tty_putc(010);
                           metal_tty_putc(' ');
                           metal_tty_putc(010);)
            continue;
        } else if (c != '\r' && c != '\n') {
            continue;
        } else {
            BACKUP_INDICES(metal_tty_putc(c);
                           metal_tty_putc_raw('\n');)
            tty_buffer[end_index] = '\n';
            return;
        }
    }
    switch(c) {
      /* Entered newline character, end the buffer presentation */
      case '\r':
      case '\n':
        BACKUP_INDICES(metal_tty_putc(c);
                       metal_tty_putc_raw('\n');)
        tty_buffer[end_index] = '\n';
        return;

      /* Backspace */
      case 010:
      /* Del */
      case 0177:
        if (BUFFER_LEN) {
          DECREASE_INDEX(end_index);
          BACKUP_INDICES(metal_tty_putc(010);
                         metal_tty_putc(' ');
                         metal_tty_putc(010);)
        }
        break;
        
      /* Other "normal" input */  
      default:
        BACKUP_INDICES(metal_tty_putc(c);)
        tty_buffer[end_index] = c;
        INCREASE_INDEX(end_index);
        break;
    }
  }
}

ssize_t
_read(int file, void *ptr, size_t len)
{
  char *cptr = ptr;

  /* If we got back -1, no default input device is available */
  if (file != STDIN_FILENO) {
    errno = EBADF;
    return -1;
  }
 
  /* Present an editable buffer */
  present_buffer();
  int ret_len = BUFFER_LEN + 1;

  /* Store the value in the buffer into ptr */
  while (start_index != end_index) {
    *cptr = tty_buffer[start_index];
    cptr++;
    INCREASE_INDEX(start_index);
  }
  *cptr = tty_buffer[end_index];

  return ret_len;
}
