#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Monster.h"
#include "Item.h"




class Character {

public:
Character(std::string name);
std::string getName() const;
std::vector<Monster*> getMonsters() const;
int getMonsterCount() const;
bool hasMonsters() const;


void addMonster(Monster* monster);
void swapMonster(int index, Monster* newMonster);
void removeDefeatedMonsters();
void removeMonster(int index);

void addItem(Item* item);
void swapItem(int index, Item* item);
void removeItem(int index);
std::vector<Item*> getInventory() const;
int getInventoryCount() const;
std::vector<Monster*> getAllMonsters() const;

virtual ~Character();

protected:
std::string name;
std::vector<Monster*> monsters;
std::vector<Item*> inventory;

};
 