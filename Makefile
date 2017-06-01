LDFLAGS=-framework CoreFoundation -framework IOKit

.PHONY: all clean nuke

all: iostat

iostat: main.o iostat.o

main.o iostat.o: iostat.h

clean:
	rm -f *.o

nuke: clean
	rm -f iostat
