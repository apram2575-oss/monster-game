#pragma once
#include <string>
using namespace std;

class Monster {
public:
Monster(string name, int health, int strength);

string getName() const;
int getHealth() const;
int getStrength() const;

bool isAlive() const;
void takeDamage(int damage);
virtual void attack(Monster& target);

~Monster();

private:
string name;
int health;
int strength;

};
