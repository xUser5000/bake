#include <iostream>
#include <fstream>

#include "common/bake.h"

#define BAKEFILE_NAME "Bakefile"

int main(int argc, char* argv[]) {
    std::ifstream in(BAKEFILE_NAME);
    if (!in.is_open()) {
        std::cout << "Could not open Bakefile!\n";
        return 1;
    }
    bake(in);
    in.close();
    return 0;
}
