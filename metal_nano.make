# Copyright 2020 SiFive, Inc
# Copyright © 2020 Keith Packard
#
# Licensed under the Apache License, Version 2.0 (the “License”); you
# may not use this file except in compliance with the License. A copy
# of this license can be found in the LICENSE.Apache2 file included
# with this software. You may also obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an “AS IS” BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied. See the License for the specific language governing
# permissions and limitations under the License.
#
METAL_HELPER_SRC = \
	$(FREEDOM_METAL)/gloss/nanosleep.c \
	$(FREEDOM_METAL)/gloss/sys_access.c \
	$(FREEDOM_METAL)/gloss/sys_chdir.c \
	$(FREEDOM_METAL)/gloss/sys_chmod.c \
	$(FREEDOM_METAL)/gloss/sys_chown.c \
	$(FREEDOM_METAL)/gloss/sys_clock_gettime.c \
	$(FREEDOM_METAL)/gloss/sys_close.c \
	$(FREEDOM_METAL)/gloss/sys_execve.c \
	$(FREEDOM_METAL)/gloss/sys_exit.c \
	$(FREEDOM_METAL)/gloss/sys_faccessat.c \
	$(FREEDOM_METAL)/gloss/sys_fork.c \
	$(FREEDOM_METAL)/gloss/sys_fstat.c \
	$(FREEDOM_METAL)/gloss/sys_fstatat.c \
	$(FREEDOM_METAL)/gloss/sys_ftime.c \
	$(FREEDOM_METAL)/gloss/sys_getcwd.c \
	$(FREEDOM_METAL)/gloss/sys_getpid.c \
	$(FREEDOM_METAL)/gloss/sys_gettimeofday.c \
	$(FREEDOM_METAL)/gloss/sys_isatty.c \
	$(FREEDOM_METAL)/gloss/sys_kill.c \
	$(FREEDOM_METAL)/gloss/sys_link.c \
	$(FREEDOM_METAL)/gloss/sys_lseek.c \
	$(FREEDOM_METAL)/gloss/sys_lstat.c \
	$(FREEDOM_METAL)/gloss/sys_open.c \
	$(FREEDOM_METAL)/gloss/sys_openat.c \
	$(FREEDOM_METAL)/gloss/sys_read.c \
	$(FREEDOM_METAL)/gloss/sys_sbrk.c \
	$(FREEDOM_METAL)/gloss/sys_stat.c \
	$(FREEDOM_METAL)/gloss/sys_sysconf.c \
	$(FREEDOM_METAL)/gloss/sys_times.c \
	$(FREEDOM_METAL)/gloss/sys_unlink.c \
	$(FREEDOM_METAL)/gloss/sys_utime.c \
	$(FREEDOM_METAL)/gloss/sys_wait.c \
	$(FREEDOM_METAL)/gloss/sys_write.c \
	$(FREEDOM_METAL)/gloss/crt0.S

METAL_HELPER_VPATH=$(FREEDOM_METAL)/gloss

METAL_HELPER_CFLAGS=
