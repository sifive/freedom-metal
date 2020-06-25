
DEVICETREE ?= test/qemu_sifive_e31.dts
OUTPUT_DIR ?= build

generate: virtualenv $(OUTPUT_DIR) scripts/codegen.py scripts/intgen.py
	. venv/bin/activate && python3 scripts/codegen.py \
		--dts $(DEVICETREE) \
		--output-dir $(OUTPUT_DIR)
	. venv/bin/activate && python3 scripts/intgen.py \
		--dts $(DEVICETREE) \
		--output-dir $(OUTPUT_DIR)

$(OUTPUT_DIR):
	mkdir -p $@

virtualenv: venv/bin/activate

venv/bin/activate: requirements.txt
	python3 -m venv venv
	. venv/bin/activate
	python3 -m pip install -r requirements.txt

METAL_SRCS = $(wildcard src/*.c src/*.S src/drivers/*.c) build/src/interrupt_table.c build/src/jump_table.S
GLOSS_SRCS = $(wildcard gloss/*.c)

build/src/interrupt_table.c: generate
build/src/jump_table.S: generate

CFLAGS = -march=rv32imac -mabi=ilp32 -Ibuild -I. -nostartfiles -nostdlib --specs=nano.specs -Ttest/qemu_sifive_e31.lds

hello: hello/hello.c $(METAL_SRCS) $(GLOSS_SRCS)
	riscv64-unknown-elf-gcc $(CFLAGS) $^ -o $@

clean:
	-rm -rf venv
	-rm -r $(OUTPUT_DIR)
