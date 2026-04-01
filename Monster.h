#pragma once
#include <string>
using namespace std;

class Monster {
public:
Monster(string name, int health, int strength, string specialEffect = "", int effectChance = 0, int effectDuration = 0, int critChance = 10, float critMultiplier = 1.5);

string getName() const;
int getHealth() const;
int getStrength() const;
string getStatus() const;
int getMaxHealth() const;

bool isAlive() const;
void takeDamage(int damage);
virtual void attack(Monster& target);
void applyStatusEffect(string effect, int duration);
void effectStatus();
void reset();

~Monster();

private:
string name;
int health;
int strength;
string currentStatus;
int statusDuration;
int effectChance;
int effectDuration;  
string specialEffect;
int maxHealth;
int critChance;
float critMultiplier;


};
