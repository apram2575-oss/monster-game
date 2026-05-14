#pragma once

#include <string>


class Monster;

class Item {
private:
    std::string name;
    std::string description;
    std::string effect;
    std::string target;
    int value;
    std::string statusEffect;
    bool reusable;

public:
    Item(std::string name, std::string description,
         std::string effect, std::string target,
         int value, std::string statusEffect = "",
         bool reusable = false);
    ~Item();

    std::string getName() const;
    std::string getDescription() const;
    std::string getEffect() const;
    std::string getTarget() const;
    int getValue() const;
    std::string getStatusEffect() const;
    bool isReusable() const;

    void use(Monster* target);
};
