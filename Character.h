#pragma once
#include <string>
#include <vector>

using namespace std;

class Monster {

public:
Character(string name);
string getName() const;
vector<Monster*> getMonsters() const;
int getMonsterCount() const;
bool hasMonsters() const;


void addMonster(Monster* monster);
void swapMonster(int index, Monster* newMonster);
void removeDefeatedMonsters();

~Character();

private:
string name;
vector<Monster*> monsters;

};
 