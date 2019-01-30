# Copyright (c) 2019 SiFive Inc.
#
# Documentation Build

.PHONY: all
all: html pdf

##########################
# Build PDF Documentation
##########################

.PHONY: pdf
pdf: latex/FreedomMetal.pdf

latex/FreedomMetal.pdf: latex/FreedomMetal.tex
	$(MAKE) -C latex

latex/FreedomMetal.tex: xml/index.xml sphinx/*
	sphinx-build -b latex sphinx latex

.PHONY: clean-pdf
clean-pdf:
	rm -rf latex
clean: clean-pdf

###########################
# Build HTML Documentation
###########################

.PHONY: html
html: html/index.html

# Use Sphinx to build HTML from Doxygen XML
html/index.html: xml/index.xml sphinx/*
	sphinx-build -b html sphinx html

##########################
# Build XML Documentation
##########################

xml/index.xml: ../metal/*.h
	cd .. && doxygen

.PHONY: clean
clean:
	rm -rf xml
