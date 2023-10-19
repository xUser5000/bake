IDIR=include
CC=gcc
CFLAGS=-I$(IDIR) -g
CFLAGS_EXTRA=-Wall -Wextra -Wpedantic \
	-Wformat=2 -Wno-unused-parameter -Wshadow \
	-Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
	-Wredundant-decls -Wnested-externs -Wmissing-include-dirs \
	-Wjump-misses-init -Wlogical-op
EXEC=bake
VALGRIND=valgrind --leak-check=yes

SDIR=src
BDIR=build
TDIR=test

LIBS=-lm -lpcre

_DEPS=common.h tokenizer.h list.h rule.h parser.h e4c_lite.h map.h graph.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=tokenizer.o list.o rule.o parser.o e4c_lite.o map.o graph.o
OBJ=$(patsubst %,$(BDIR)/%,$(_OBJ))

_TESTS=test_tokenizer test_list test_parser test_map test_graph
TESTS=$(patsubst %,$(BDIR)/%,$(_TESTS))

# mkdir the build directory when Makefile is parsed
$(shell mkdir -p $(BDIR))

all: $(EXEC) $(TESTS)

# run executable
run: $(EXEC)
	$(BDIR)/$(EXEC)

# build executable
$(EXEC): $(SDIR)/main.c $(OBJ)
	$(CC) -o $(BDIR)/$@ $(SDIR)/main.c $(OBJ) $(CFLAGS) $(CFLAGS_EXTRA) $(LIBS)

# build object files from source files
$(BDIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(CFLAGS_EXTRA) $(LIBS)

# run all tests
run-tests: $(TESTS)
	@for file in $(TESTS); do \
		echo "\n\nRunning $$file...\n"; \
        $(VALGRIND) $$file; \
    done

# run a particular test
run-test-%: $(BDIR)/test_%
	$(VALGRIND) $(BDIR)/test_$*

# compiling tests
tests: $(TESTS)

$(BDIR)/test_%: $(TDIR)/test_%.c $(OBJ) $(DEPS)
	$(CC) -o $@ $(TDIR)/test_$*.c $(OBJ) $(CFLAGS) -g $(LIBS)

# clean the build directory
.PHONY: clean
clean:
	rm -rf $(BDIR)
