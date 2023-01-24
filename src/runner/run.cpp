//
// Created by xuser5000 on 25/01/23.
//

#include "runner/run.h"

int run(std::string &command) {
    return system(command.c_str());
}

void run(std::vector<std::string> &commands) {
    for (std::string &cmd: commands) {
        if (!WIFEXITED(run(cmd))) {
            return;
        }
    }
}
