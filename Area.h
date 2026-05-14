#pragma once
#include <string>
#include "Enemy.h"


class Area {

public:

Area(std::string name, std::string description);
std::string getName() const;
std::string getDescription() const;
void addEnemy(Enemy* enemy);
std::vector<Enemy*> getEnemies() const;
void removeEnemy(Enemy* enemy);

~Area();

private:
std::string name;
std::string description;
std::vector<Enemy*> enemies;

};