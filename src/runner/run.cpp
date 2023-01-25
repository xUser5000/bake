//
// Created by xuser5000 on 25/01/23.
//

#include "runner/run.h"

int run(std::string command) {
    return system(command.c_str());
}

int run(std::vector<std::string> commands) {
    for (std::string &cmd: commands) {
        int res = run(cmd);
        if (!WIFEXITED(res)) {
            return res;
        }
    }
    return 0;
}
