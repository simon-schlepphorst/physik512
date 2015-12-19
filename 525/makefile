# Copyright © 2013-2015 Martin Ueding <dev@martin-ueding.de>
# Licensed under The MIT License

number = 525

all_eps := $(wildcard Daten/*.GZ)
all_pdf := $(all_eps:Daten/%.GZ=_build/%.pdf)

_build/%.ps : Daten/%.GZ
	mkdir -p _build
	gzip -d -c $< > $@

%-temp.pdf : %.ps
	ps2pdf $< $@

%-temp2.pdf : %-temp.pdf
	#for pdfjam:
	#pdf270 -o $@-temp2.pdf $@-temp.pdf
	#
	#for pdftk:
	pdftk $< cat 1east output $@

%.pdf : %-temp2.pdf
	pdfcrop $< $@

all: $(all_pdf)

include ../makefile.in
