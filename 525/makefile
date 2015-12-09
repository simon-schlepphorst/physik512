# Copyright © 2013-2015 Martin Ueding <dev@martin-ueding.de>
# Licensed under The MIT License

number = 525

%.eps : %.GZ
	gzip -d -c $< > $@.eps

%.pdf : %.eps
	ps2pdf $< $@

include ../makefile.in
