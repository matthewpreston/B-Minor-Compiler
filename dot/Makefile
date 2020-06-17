CFLAGS=`pkg-config libgvc --cflags` -Wall -g -O2
LDFLAGS=`pkg-config libgvc --libs`

define compile
	cc -o $@ $< $(LDFLAGS)
endef

all: dot demo simple example

dot: dot.o
	$(call compile)
demo: demo.o
	$(call compile)
simple: simple.o
	$(call compile)
example: example.o
	$(call compile)

clean:
	rm -rf *.o dot demo simple example *.png
