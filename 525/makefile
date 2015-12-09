# Copyright © 2013-2015 Martin Ueding <dev@martin-ueding.de>
# Licensed under The MIT License

number = 525

all_eps := $(wildcard Daten/*.GZ)
all_pdf := $(all_eps:Daten/%.GZ=_build/%.pdf)

_build/%.ps : Daten/%.GZ
	gzip -d -c $< > $@

%.pdf : %.ps
	ps2pdf $< $@-temp.pdf
	pdf270 -o $@-temp2.pdf $@-temp.pdf
	pdfcrop $@-temp2.pdf $@

all: $(all_pdf)

include ../makefile.in
