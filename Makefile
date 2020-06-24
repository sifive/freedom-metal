
DEVICETREE ?= test/qemu_sifive_e31.dts
OUTPUT_DIR ?= build

generate: virtualenv $(OUTPUT_DIR)
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

clean:
	-rm -rf venv
	-rm -r $(OUTPUT_DIR)
