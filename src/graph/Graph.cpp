//
// Created by xuser5000 on 22/01/23.
//

#include "graph/Graph.h"

Graph::Graph() = default;

Graph::Graph(const std::map<std::string, std::vector<std::string>> &adj_list) {
    adj_list_ = adj_list;
}

void Graph::setChildren(const std::string &node, const std::vector<std::string> &children) {
    adj_list_[node] = children;
}

std::vector<std::string> Graph::getChildren(const std::string &node) {
    return adj_list_[node];
}

bool Graph::isCyclic() {
    std::map<std::string, int> mark;
    bool cyclic = false;
    for (auto &p: adj_list_) {
        const std::string &node = p.first;
        if (mark[node] == 0) {
            cyclic = cyclic || isCyclic(node, mark);
        }
    }
    return cyclic;
}

bool Graph::isCyclic(const std::string &cur_node, std::map<std::string, int> &mark) {
    bool ans = false;
    mark[cur_node] = 1;
    for (const std::string &child: adj_list_[cur_node]) {
        switch (mark[cur_node]) {
            case 0:
                ans = ans || isCyclic(child, mark);
                break;
            case 1:
                return true;    // come across a node in the current stack -> there is a cycle
            case 2:
                continue;
        }
    }
    mark[cur_node] = 2;
    return ans;
}

std::vector<std::string> Graph::topologicalOrder(std::string root_node) {
    std::vector<std::string> order;
    std::map<std::string, int> mark;
    topologicalOrder(root_node, mark, order);
    return order;
}

void Graph::topologicalOrder(std::string &cur_node, std::map<std::string, int> &mark, std::vector<std::string> &list) {
    for (std::string &child: adj_list_[cur_node]) {
        if (mark[child] == 0) {
            topologicalOrder(child, mark, list);
        }
    }
    mark[cur_node] = 1;
    list.push_back(cur_node);
}
