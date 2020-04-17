# Copyright (c) 2020 SiFive Inc.
# SPDX-License-Identifier: Apache-2.0

all: virtualenv

.PHONY: virtualenv
virtualenv: venv/.stamp

venv/.stamp: venv/bin/activate requirements.txt
	. venv/bin/activate && pip install --upgrade pip
	. venv/bin/activate && pip install -r requirements.txt
	@echo "Remember to source venv/bin/activate!"
	touch $@

venv/bin/activate:
	python3 -m venv venv

.PHONY: test-lint
test-lint: virtualenv
	. venv/bin/activate && pylint *.py

.PHONY: test
test: test-lint

clean:
	-rm -rf venv __pycache__
