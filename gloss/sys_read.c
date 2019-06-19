#include <metal/tty.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

/* Parameters for the circular input buffer */
#define TTY_BUFFER_LEN 127
static char tty_buffer[TTY_BUFFER_LEN];
static uint8_t start_index;
static uint8_t end_index;
#define increase_index(n) (n = (n + 1) % TTY_BUFFER_LEN)
#define decrease_index(n) (n = (n + TTY_BUFFER_LEN - 1) % TTY_BUFFER_LEN)
#define buffer_len (end_index > start_index ? \
                                        end_index - start_index : \
                                        end_index + TTY_BUFFER_LEN - start_index \
                                        )

static void present_buffer(void) {
  signed char c;
  int int_buffer;
  while (1) {
    metal_tty_getc(&int_buffer);
    /* If input is nothing, do nothing */
    if (int_buffer == -1) {
      continue;
    }
    switch((c = int_buffer)) {
      /* Entered newline character, end the buffer presentation */
      case '\r':
      case '\n':
        metal_tty_putc(c);
        metal_tty_putc_raw('\n');
        tty_buffer[end_index] = '\n';
        increase_index(end_index);
        return;

      /* Backspace */
      case 010:
      /* Del */
      case 0177:
        if (buffer_len) {
          decrease_index(end_index);
          metal_tty_putc(010);
          metal_tty_putc(' ');
          metal_tty_putc(010);
        }
        break;
        
      /* Other "normal" input */  
      default:
        tty_buffer[end_index] = c;
        metal_tty_putc(c);
        increase_index(end_index);
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
  int ret_len = buffer_len;

  /* Store the value in the buffer into ptr */
  while (start_index != end_index) {
    *cptr = tty_buffer[start_index];
    cptr++;
    increase_index(start_index);
  }

  return ret_len;
}
