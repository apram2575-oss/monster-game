#include "Enemy.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Enemy::Enemy(string name, string area) : Character(name), area(area) {}



string Enemy::getArea() const 
{
    return area;
}



Enemy::~Enemy() {}

