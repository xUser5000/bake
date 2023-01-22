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

    Rule(const std::string &target, const std::vector<std::string> &dependencies,
         const std::vector<std::string> &recipes) : target_(target), dependencies_(dependencies), recipes_(recipes) {}

    const std::string &getTarget() const { return target_; }

    void setTarget(const std::string &target) { target_ = target; }

    const std::vector<std::string> &getDependencies() const { return dependencies_; }

    void setDependencies(const std::vector<std::string> &dependencies) { dependencies_ = dependencies; }

    const std::vector<std::string> &getRecipes() const { return recipes_; }

    void setRecipes(const std::vector<std::string> &recipes) { recipes_ = recipes; }

private:
    std::string target_;
    std::vector<std::string> dependencies_;
    std::vector<std::string> recipes_;
};

#endif //BAKE_RULE_H
