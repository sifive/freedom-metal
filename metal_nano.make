METAL_HELPER_C = \
	nanosleep.c \
	sys_access.c \
	sys_chdir.c \
	sys_chmod.c \
	sys_chown.c \
	sys_clock_gettime.c \
	sys_close.c \
	sys_execve.c \
	sys_exit.c \
	sys_faccessat.c \
	sys_fork.c \
	sys_fstat.c \
	sys_fstatat.c \
	sys_ftime.c \
	sys_getcwd.c \
	sys_getpid.c \
	sys_gettimeofday.c \
	sys_isatty.c \
	sys_kill.c \
	sys_link.c \
	sys_lseek.c \
	sys_lstat.c \
	sys_open.c \
	sys_openat.c \
	sys_read.c \
	sys_sbrk.c \
	sys_stat.c \
	sys_sysconf.c \
	sys_times.c \
	sys_unlink.c \
	sys_utime.c \
	sys_wait.c \
	sys_write.c

METAL_HELPER_S = \
	crt0.S

METAL_HELPER_VPATH=$(FREEDOM_METAL)/gloss
