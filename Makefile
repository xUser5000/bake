IDIR=include
CC=gcc
CFLAGS=-I$(IDIR)
EXEC=bake

SDIR=src
BDIR=build
TDIR=test

LIBS=-lm -lpcre

_DEPS=tokenizer.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=tokenizer.o
OBJ = $(patsubst %,$(BDIR)/%,$(_OBJ))

_TESTS=test_tokenizer
TESTS=$(patsubst %,$(BDIR)/%,$(_TESTS))

# mkdir the build directory when Makefile is parsed
$(shell mkdir -p $(BDIR))

all: $(EXEC)

# build executable
$(EXEC): $(SDIR)/main.c $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

# build object files from source files
$(BDIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

# testing
test: $(TESTS)

$(BDIR)/%: test_dummy $(TDIR)/%.c $(OBJ) $(DEPS)
	$(CC) -o $@ $(TDIR)/$*.c $(OBJ) $(CFLAGS) $(LIBS)
	$(BDIR)/$*

# dummy rule to mark test targets out-of-date everytime they run
.PHONY: test_dummy
test_dummy:
	@true

# clean the build directory
.PHONY: clean
clean:
	rm -f $(BDIR)/*

