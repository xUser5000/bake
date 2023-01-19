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
    Rule(const std::string &target, const std::string &dependency,
         const std::string &recipe) : target_(target), dependency_(dependency), recipe_(recipe) {}

    const std::string &getTarget() const { return target_; }

    void setTarget(const std::string &target) { target_ = target; }

    const std::string &getDependency() const { return dependency_; }

    void setDependency(const std::string &dependency) { dependency_ = dependency; }

    const std::string &getRecipe() const { return recipe_; }

    void setRecipe(const std::string &recipe) { recipe_ = recipe; }

private:
    std::string target_;
    std::string dependency_;
    std::string recipe_;
};

#endif //BAKE_RULE_H
