analysis: analysis.C
	g++ --std=c++11 -ggdb -Wl,--no-as-needed `root-config --libs --ldflags` -I `root-config --incdir`  $^ -o $@

root = $(wildcard *.root)
js = $(root:=.js)

test:
	echo $(root)

js: $(js)

%.root.js: %.root analysis
	echo '.q' | ./analysis $<
