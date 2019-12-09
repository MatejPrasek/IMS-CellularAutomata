# Description: Create client for SIP protokol
# File:        Makefile

CC              = g++
CFLAGS         = -g -lGLU -lGL -lglut
OBJ_FILES      = IMS.o Blank.o FemaleVole.o MaleVole.o Poison.o Predator.o Vole.o
#LIBRARIES       = -static-libstdc++

all: IMS

run: experiment1 experiment2 experiment3 experiment4 experiment5 experiment6

IMS:	$(OBJ_FILES)
		$(CC) $(OBJ_FILES) -o $@ $(CFLAGS)

IMS.o: IMS.cpp Blank.h FemaleVole.h MaleVole.h Poison.h Predator.h Vole.h Cell.h CellTypes.h
	$(CC) -o $@ -c $<  $(CFLAGS)

Blank.o: Blank.cpp Blank.h
	$(CC) -o $@ -c $<  $(CFLAGS)

FemaleVole.o: FemaleVole.cpp FemaleVole.h
	$(CC) -o $@ -c $<  $(CFLAGS)

MaleVole.o: MaleVole.cpp MaleVole.h
	$(CC) -o $@ -c $<  $(CFLAGS)

Poison.o: Poison.cpp Poison.h
	$(CC) -o $@ -c $<  $(CFLAGS)

Predator.o: Predator.cpp Predator.h
	$(CC) -o $@ -c $<  $(CFLAGS)

Vole.o: Vole.cpp Vole.h
	$(CC) -o $@ -c $<  $(CFLAGS)

experiment1:
	./simulator -v 2 -d 5 -t 100 -w 50 -r 10 -f experiment1

experiment2:
	./simulator -v 4 -d 5 -t 100 -w 50 -r 10 -f experiment2

experiment3:
	./simulator -v 2 -d 5 -t 100 -w 50 -r 10 -e 10 -f experiment3

experiment4:
	./simulator -v 2 -d 5 -t 100 -w 50 -e 10 -x 90 -m -f experiment4

experiment5:
	./simulator -d 5 -s 100 -t 100 -w 50 -r 20 -f experiment5

experiment6:
	./simulator -v 2 -d 5 -s 100 -t 100 -w 50 -r 10 -k -f experiment6

clean:
	rm -f *.o IMS experiment*

.PHONY: clean
