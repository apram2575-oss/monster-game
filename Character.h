#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Monster.h"

using namespace std;

class Character {

public:
Character(string name);
string getName() const;
vector<Monster*> getMonsters() const;
int getMonsterCount() const;
bool hasMonsters() const;


void addMonster(Monster* monster);
void swapMonster(int index, Monster* newMonster);
void removeDefeatedMonsters();

virtual ~Character();

protected:
string name;
vector<Monster*> monsters;

};
 