CFLAGS = -Wall -I.
GCC = gcc

all: ssi 

ssi: build/ssi.o build/bgjobs.o build/emalloc.o build/node.o build/linkedlist.o build/historyfile.o | db
	${GCC} ${CFLAGS} build/ssi.o build/bgjobs.o build/emalloc.o build/node.o build/linkedlist.o build/historyfile.o -o ssi -lreadline

build/ssi.o: ssi.c headers/emalloc.h headers/node.h headers/linkedlist.h headers/historyfile.h headers/bgjobs.h | build
	${GCC} ${CFLAGS} -c ssi.c -o build/ssi.o
build/emalloc.o: src/emalloc.c headers/emalloc.h | build
	${GCC} ${CFLAGS} -c src/emalloc.c -o build/emalloc.o

build/linkedlist.o: src/linkedlist.c headers/emalloc.h headers/node.h | build
	${GCC} ${CFLAGS} -c src/linkedlist.c -o build/linkedlist.o

build/node.o: src/node.c headers/node.h | build
	${GCC} ${CFLAGS} -c src/node.c -o build/node.o

build/bgjobs.o: src/bgjobs.c headers/bgjobs.h headers/emalloc.h | build
	${GCC} ${CFLAGS} -c src/bgjobs.c -o build/bgjobs.o

build/historyfile.o: src/historyfile.c headers/linkedlist.h headers/emalloc.h | build 
	${GCC} ${CFLAGS} -c src/historyfile.c -o build/historyfile.o

db:
	mkdir -p db
	touch db/history.txt

build:
	mkdir -p build

clean:
	rm -f build/*.o ssi
