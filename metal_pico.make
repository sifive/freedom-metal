METAL_HELPER_SRC = \
	$(FREEDOM_METAL)/pico/iob.c \
	$(FREEDOM_METAL)/gloss/sys_sbrk.c \
	$(FREEDOM_METAL)/gloss/sys_exit.c \
	$(FREEDOM_METAL)/gloss/sys_times.c \
	$(FREEDOM_METAL)/gloss/sys_sysconf.c \
	$(FREEDOM_METAL)/gloss/sys_gettimeofday.c \
	$(FREEDOM_METAL)/gloss/sys_clock_gettime.c \
	$(FREEDOM_METAL)/gloss/sys_write.c \
	$(FREEDOM_METAL)/gloss/sys_getpid.c \
	$(FREEDOM_METAL)/gloss/sys_kill.c \
	$(FREEDOM_METAL)/gloss/crt0.S

METAL_HELPER_VPATH=$(FREEDOM_METAL)/pico:$(FREEDOM_METAL)/gloss

METAL_HELPER_CFLAGS=-DPICOLIBC_INTEGER_PRINTF_SCANF
