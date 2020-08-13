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

ifeq ($(USE_FLOATING_PRINTF),)
METAL_HELPER_CFLAGS=-DPICOLIBC_INTEGER_PRINTF_SCANF
endif
