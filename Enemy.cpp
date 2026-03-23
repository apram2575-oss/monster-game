#include "Enemy.h"
#include <iostream>
#include <cstdlib>

Enemy::Enemy(string name, int difficulty, string area) : Character(name), difficulty(difficulty), area(area) {}

int Enemy::getDifficulty() const 
{
    return difficulty;
}

string Enemy::getArea() const 
{
    return area;
}



Enemy::~Enemy() {}

