IDIR=include
CC=gcc
CFLAGS=-I$(IDIR)
EXEC=bake

SDIR=src
BDIR=build
LDIR=lib

LIBS=-lm -lpcre

_DEPS=tokenizer.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = tokenizer.o main.o
OBJ = $(patsubst %,$(BDIR)/%,$(_OBJ))

# mkdir the build directory when Makefile is parsed
$(shell mkdir -p $(BDIR))

all: $(EXEC)

# build executable
$(EXEC): $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(LIBS)

# build object files from source files
$(BDIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

# clean the build directory
.PHONY: clean
clean:
	rm -f $(BDIR)/*.o *~ core $(IDIR)/*~
	rm -f $(BDIR)/$(EXEC)
