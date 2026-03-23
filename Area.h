#pragma once
#include <string>
#include "Enemy.h"

using namespace std;

class Area {

public:

Area(string name, string description);
string getName() const;
string getDescription() const;
void addEnemy(Enemy* enemy);
vector<Enemy*> getEnemies() const;

~Area();

private:
string name;
string description;
vector<Enemy*> enemies;

};