# Bake
Bake is an eductaional clone of GNU Make written in C. The aim of the project is to learn more about C, build systems, tokenization, parsing, graph theory, parallel computation, and more!

# Features
- [x] Execute all targets in a Bakefile
- [x] Specify a single target to execute
- [x] Detect and report cycles in the dependency graph
- [x] Raise an error when target is not defined

# TODO
- [ ] Avoid re-executing targets that are up to date
- [ ] Parallelize target execution

# Installing Dependencies
Bake is only supported on Linux. It has been tested on Ubuntu 20.04 LTS.

To compile Bake, you will need the following:
- GNU Make (unironically)
- GCC (GNU Compiler Collection)
- pcre (package for Perl Compatible Regular Expressions)
  - To install: `sudo apt update && sudo apt install libpcre3 libpcre3-dev`

# Compiling
- To compile Bake, run `make` in the root directory of the project. The following binaries will be created in the `build` directory:
  - `bake` executable.
  - some test binaries named `build/test_<testname>`
- To generate debug symbols when compiling bake, run `make DEBUG=1`.
- Run `make clean` in the root directory of the project to remove all build artifacts.


# Testing
- Run `make run-tests` in the root directory of the project to run the test suite.
- To run a particular test, run `make run-test-<testname>` (without the `.c` extension).


# Running
- To run Bake, run `./build/bake`. Note that this will execute `Bakefile` in the root of the project.
