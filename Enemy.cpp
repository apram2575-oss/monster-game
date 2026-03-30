#include "Enemy.h"
#include <iostream>
#include <cstdlib>

Enemy::Enemy(string name, string area) : Character(name), area(area) {}



string Enemy::getArea() const 
{
    return area;
}



Enemy::~Enemy() {}

