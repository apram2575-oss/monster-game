#include "Area.h"


Area::Area(string name, string description) : name(name), description(description) {}

string Area::getName() const 
{
    return name;
}

string Area::getDescription() const 
{
    return description;
}

void Area::addEnemy(Enemy* enemy) 
{
    enemies.push_back(enemy);
}

vector<Enemy*> Area::getEnemies() const 
{
    return enemies;
}

void Area::removeEnemy(Enemy* enemy) 
{
    for (auto it = enemies.begin(); it != enemies.end(); ++it) 
    {
        if (*it == enemy) 
        {
            enemies.erase(it); // Remove the pointer from the vector
            return;
        }
    }
}

Area::~Area() 
{
    for (Enemy* e : enemies) 
    {
        delete e;
    }
}