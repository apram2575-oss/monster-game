#pragma once

#include "Monster.h"
#include "Item.h"
#include <string>
#include <vector>

class Character;

class Cave {
private:
    std::string name;
    std::string description;
    std::vector<Monster*> monsters;
    std::vector<Item*> rewardItems;
    bool cleared;

public:
    Cave(std::string name, std::string description);
    ~Cave();

    std::string getName() const;
    std::string getDescription() const;
    std::vector<Monster*> getMonsters() const;
    std::vector<Item*> getRewardItems() const;
    bool isCleared() const;

    void addMonster(Monster* monster);
    void addRewardItem(Item* item);
    void removeDefeatedMonsters();
    bool hasMonsters() const;
    void clear();
};
