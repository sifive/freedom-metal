
DEVICETREE ?= test/qemu_sifive_e31.dts
OUTPUT_DIR ?= build

generate: virtualenv $(OUTPUT_DIR) scripts/codegen.py
	. venv/bin/activate && python3 scripts/codegen.py \
		--dts $(DEVICETREE) \
		--template-path templates sifive-blocks/templates \
		--output-dir $(OUTPUT_DIR)

$(OUTPUT_DIR):
	mkdir -p $@

virtualenv: venv/bin/activate

venv/bin/activate: requirements.txt
	python3 -m venv venv
	. venv/bin/activate && python3 -m pip install -r requirements.txt

METAL_SRCS = $(wildcard src/*.c src/*.S src/drivers/*.c sifive-blocks/src/drivers/*.c $(OUTPUT_DIR)/src/*.c $(OUTPUT_DIR)/src/*.S)
GLOSS_SRCS = $(wildcard gloss/*.c) $(wildcard gloss/*.S)

$(OUTPUT_DIR)/src/interrupt_table.c: generate
$(OUTPUT_DIR)/src/jump_table.S: generate

LIBC ?= picolibc
LIBC_FLAGS = --specs=$(LIBC).specs
ifeq ($(LIBC),picolibc)
	LIBC_FLAGS += --oslib=semihost
endif

CFLAGS=-march=rv32imac -mabi=ilp32 -mcmodel=medlow -ffunction-sections -fdata-sections -I$(OUTPUT_DIR) -I. -Isifive-blocks $(LIBC_FLAGS) -DMTIME_RATE_HZ_DEF=32768 -Og -g
CFLAGS += -DMETAL_HLIC_VECTORED
LDFLAGS=-Wl,--gc-sections -Wl,-Map,hello.map -nostartfiles -Ttest/qemu_sifive_e31.lds
LDLIBS=-lm

CC=riscv64-unknown-elf-gcc

hello: hello-src/hello.c $(METAL_SRCS) $(GLOSS_SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

.PHONY: simulate
simulate: hello
	qemu-system-riscv32 -readconfig test/qemu_sifive_e31.cfg -kernel hello -nographic

clean:
	-rm -rf venv
	-rm -r $(OUTPUT_DIR)
	-rm -f hello hello.map
