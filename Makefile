# Makefile for Phylogenetic Class Library
CC		:= g++ #icc #
CFLAGS	:= -c -Wall -fPIC -std=c++11
LFLAGS	:= -L./
DBGF	:= -g -DDEBUG
SRCDIRS := ./src/
INCDIRS := -I./src/
SLNAME	:= libpcl.a
DLNAME	:= libpcl.so
OBJ		:= *.o
SRC		:= *.cpp

all: $(SLNAME) $(DLNAME)

release: CFLAGS := $(CFLAGS) -O3
release: all

debug:	CFLAGS := $(CFLAGS) $(DBGF) 
debug:  LFLAGS := $(LFLAGS)
debug:	all

$(SLNAME) : $(OBJ)
	ar rvs $(SLNAME) $(OBJ)

$(DLNAME) : $(OBJ)
	$(CC) -shared -o $(DLNAME) $(OBJ) $(LFLAGS)

$(OBJ) : $(SRCDIRS)$(SRC)
	$(CC) $(INCDIRS) $(CFLAGS) $(SRCDIRS)$(SRC)

clean:
	rm *.o
