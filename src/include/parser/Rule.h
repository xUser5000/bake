//
// Created by xuser5000 on 19/01/23.
//

#ifndef BAKE_RULE_H
#define BAKE_RULE_H

#include <string>
#include <vector>

class Rule {
public:
    Rule() = default;

    Rule(std::string target, std::vector<std::string> dependencies,
         std::vector<std::string> recipes) : target_(target), dependencies_(dependencies), recipes_(recipes) {}

    bool operator==(const Rule &rhs) const {
        return target_ == rhs.target_ &&
               dependencies_ == rhs.dependencies_ &&
               recipes_ == rhs.recipes_;
    }

    bool operator!=(const Rule &rhs) const {
        return !(rhs == *this);
    }

    std::string getTarget() { return target_; }

    void setTarget(std::string target) { target_ = target; }

    std::vector<std::string> getDependencies() { return dependencies_; }

    void setDependencies(std::vector<std::string> dependencies) { dependencies_ = dependencies; }

    std::vector<std::string> getRecipes() { return recipes_; }

    void setRecipes(std::vector<std::string> recipes) { recipes_ = recipes; }

private:
    std::string target_;
    std::vector<std::string> dependencies_;
    std::vector<std::string> recipes_;
};

#endif //BAKE_RULE_H
