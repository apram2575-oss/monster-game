#pragma once
#include <string>
#include "Character.h"
#include "Monster.h"

using namespace std;

class Enemy : public Character {

public:

Enemy(string name, string area);
int getDifficulty() const;
string getArea() const;



~Enemy();
private:

int difficulty;
string area;

};