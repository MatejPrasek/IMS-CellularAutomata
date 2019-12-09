# Description: Create client for SIP protokol
# File:        Makefile

CC              = g++
CFLAGS         = -g -lGLU -lGL -lglut
OBJ_FILES      = IMS.o Blank.o FemaleVole.o MaleVole.o Poison.o Predator.o Vole.o
#LIBRARIES       = -static-libstdc++

all: IMS

run: 
	./IMS

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

clean:
	rm -f *.o IMS experiment*

.PHONY: clean
