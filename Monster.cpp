#include "Monster.h"


Monster::~Monster() {}

Monster::Monster(string name, int health, int strength) : name(name), health(health), strength(strength) {}

string Monster::getName() const 
{
    return name;
}

int Monster::getHealth() const 
{
    return health;
}

int Monster::getStrength() const 
{
    return strength;
}

bool Monster::isAlive() const 
{
    return health > 0;
}

void Monster::takeDamage(int damage) 
{
    health -= damage;
    if (health < 0) 
    {
        health = 0;
    }
}

void Monster::attack(Monster& target) 
{
    if (isAlive()) 
    {
        target.takeDamage(strength);
    }
}

Monster::~Monster() {}