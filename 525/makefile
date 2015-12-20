# Copyright © 2013-2015 Martin Ueding <dev@martin-ueding.de>
# Licensed under The MIT License

number = 525

all_eps := $(wildcard Daten/*.GZ)
all_pdf := $(all_eps:Daten/%.GZ=_build/%.png)

all: $(all_pdf)

_build:
	mkdir -p _build

_build/%.ps: Daten/%.GZ _build
	gzip -d -c $< > $@

%-temp.pdf : %.ps
	ps2pdf $< $@

%-000.png: %-temp.pdf
	pdfimages -png $< $(@:-000.png=)

%.png: %-000.png
	convert $< -rotate 90 -negate $@

%.pdf : %-temp2.pdf
	pdfcrop $< $@

all: $(all_pdf)

include ../makefile.in
