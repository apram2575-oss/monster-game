#include "Cave.h"
#include <iostream>
using namespace std;

Cave::Cave(string name, string description) : name(name), description(description), cleared(false) {}

Cave::~Cave()
{
    for (Monster* m : monsters)
        delete m;
    for (Item* i : rewardItems)
        delete i;
}

string Cave::getName() const 
{ 
    return name; 
}
string Cave::getDescription() const 
{ 
    return description; 
}
vector<Monster*> Cave::getMonsters() const 
{ 
    return monsters; 
}
vector<Item*> Cave::getRewardItems() const 
{ 
    return rewardItems; 
}
bool Cave::isCleared() const 
{ 
    return cleared; 
}

void Cave::addMonster(Monster* monster)
{
    monsters.push_back(monster);
}

void Cave::addRewardItem(Item* item)
{
    rewardItems.push_back(item);
}

void Cave::removeDefeatedMonsters()
{
    for (auto it = monsters.begin(); it != monsters.end();)
    {
        if (!(*it)->isAlive())
        {
            it = monsters.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool Cave::hasMonsters() const
{
    for (Monster* m : monsters)
        if (m->isAlive())
            return true;
    return false;
}

void Cave::clear()
{
    cleared = true;
}