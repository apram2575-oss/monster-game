#pragma once
#include <string>
#include "Character.h"
#include "Monster.h"



class Enemy : public Character {

public:

Enemy(std::string name, std::string area);
int getDifficulty() const;
std::string getArea() const;



~Enemy();
private:

int difficulty;
std::string area;

};