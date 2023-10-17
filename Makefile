IDIR=include
CC=gcc
CFLAGS=-I$(IDIR)
CFLAGS_EXTRA=-Wall -Wextra -Wpedantic \
	-Wformat=2 -Wno-unused-parameter -Wshadow \
	-Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
	-Wredundant-decls -Wnested-externs -Wmissing-include-dirs \
	-Wjump-misses-init -Wlogical-op
EXEC=bake

SDIR=src
BDIR=build
DDIR=build-debug
TDIR=test

LIBS=-lm -lpcre

_DEPS=common.h tokenizer.h list.h rule.h parser.h e4c_lite.h map.h graph.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

OBJ=tokenizer.o list.o rule.o parser.o e4c_lite.o map.o graph.o
BOBJ=$(patsubst %,$(BDIR)/%,$(OBJ))

TESTS=test_tokenizer test_list test_parser test_map test_graph

# add -g flag if "DEBUG" argument is passed
ifeq ($(DEBUG), true)
	CFLAGS += -g
	BDIR = $(DDIR)
endif

# mkdir the build directory when Makefile is parsed
$(shell mkdir -p $(BDIR))

all: $(EXEC)

# build executable
$(EXEC): $(SDIR)/main.c $(OBJ)
	$(CC) -o $(BDIR)/$@ $(SDIR)/main.c $(BOBJ) $(CFLAGS) $(CFLAGS_EXTRA) $(LIBS)

# build object files from source files
%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $(BDIR)/$@ $< $(CFLAGS) $(CFLAGS_EXTRA) $(LIBS)

# running tests
run-test:

# compiling tests
test: $(TESTS)

test_%: test_dummy $(TDIR)/test_%.c $(OBJ) $(DEPS)
	$(CC) -o $(BDIR)/$@ $(TDIR)/$@.c $(BOBJ) $(CFLAGS) $(LIBS)
	$(BDIR)/$@

# dummy rule to mark test targets out-of-date everytime they run
.PHONY: test_dummy
test_dummy:
	@true

# clean the build directory
.PHONY: clean
clean:
	rm -rf $(BDIR)
	rm -rf $(DDIR)
