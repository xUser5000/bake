//
// Created by xuser5000 on 18/01/23.
//
#include <iostream>

#include "common/bake.h"
#include "parser/Parser.h"
#include "graph/Graph.h"
#include "runner/run.h"

void bake(std::istream &in) {
    Parser parser(in);
    auto rules = parser.getRules();
    std::map<std::string, Rule> mp;
    Graph graph;
    for (auto &rule: rules) {
        graph.setChildren(rule.getTarget(), rule.getDependencies());
        mp[rule.getTarget()] = rule;
    }
    std::vector<std::string> ordered_targets = graph.topologicalOrder(rules[0].getTarget());
    for (const auto &t: ordered_targets) {
        if (!WIFEXITED(run(mp[t].getRecipes()))) {
            std::cout << "Bake: target " << t << " could not be fully baked\n";
            return;
        }
    }
    std::cout << "Targets baked successfully!";
}
