//
// Created by xuser5000 on 22/01/23.
//

#ifndef BAKE_GRAPH_H
#define BAKE_GRAPH_H

#include <vector>
#include <map>

#include "parser/Rule.h"

class Graph {
public:
    Graph();
    Graph(const std::map<std::string, std::vector<std::string>> &adj_list);
    void setChildren(const std::string &node, const std::vector<std::string> &children);
    std::vector<std::string> getChildren(const std::string &node);
    bool isCyclic();
    std::vector<std::string> topologicalOrder(std::string &root_node);

private:
    bool isCyclic(const std::string &cur_node, std::map<std::string, int> &mark);
    void topologicalOrder(std::string &cur_node, std::map<std::string, int> &mark,
                                               std::vector<std::string> &list);

    std::map<std::string, std::vector<std::string>> adj_list_;
};


#endif //BAKE_GRAPH_H
