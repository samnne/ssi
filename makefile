CFLAGS = -Wall -I.
GCC = gcc

all: ssi

ssi: build/ssi.o build/emalloc.o build/node.o build/linkedlist.o
	${GCC} ${CFLAGS} build/ssi.o build/emalloc.o build/node.o build/linkedlist.o -o ssi

build/ssi.o: headers/emalloc.h ssi.c | build
	${GCC} ${CFLAGS} -c ssi.c -o build/ssi.o

build/emalloc.o: src/emalloc.c headers/emalloc.h | build
	${GCC} ${CFLAGS} -c src/emalloc.c -o build/emalloc.o

build/linkedlist.o: src/linkedlist.c headers/emalloc.h headers/node.h | build
	${GCC} ${CFLAGS} -c src/linkedlist.c -o build/linkedlist.o

build/node.o: src/node.c headers/node.h | build
	${GCC} ${CFLAGS} -c src/node.c -o build/node.o

build:
	mkdir -p build

clean:
	rm -f build/*.o ssi