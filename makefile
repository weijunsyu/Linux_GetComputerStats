all: ksamp

ksamp.o: ksamp.c statFcn.h
	gcc -c ksamp.c -o ksamp.o -Wall -pedantic

statfcn.o: statfcn.c statfcn.h strFcn.h
	gcc -c statfcn.c -o statfcn.o -Wall -pedantic

strfcn.o: strfcn.c strfcn.h
	gcc -c strfcn.c -o strfcn.o -Wall -pedantic

ksamp: ksamp.o statfcn.o strfcn.o
	gcc ksamp.o statfcn.o strfcn.o -o ksamp -Wall -pedantic

clean:
	rm -f ksamp ksamp.o statfcn.o strfcn.o

.PHONY: all clean
