METAL_HELPER_C = \
	iob.c \
	sys_sbrk.c \
	sys_exit.c \
	sys_times.c \
	sys_sysconf.c \
	sys_gettimeofday.c \
	sys_clock_gettime.c \
	sys_write.c

METAL_HELPER_S = \
	crt0.S

METAL_HELPER_VPATH=$(FREEDOM_METAL)/pico:$(FREEDOM_METAL)/gloss
