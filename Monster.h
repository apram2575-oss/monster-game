#pragma once
#include <string>


class Monster {
public:
Monster(std::string name, int health, int strength, std::string specialEffect = "", int effectChance = 0, int effectDuration = 0, int critChance = 10, float critMultiplier = 1.5);

std::string getName() const;
int getHealth() const;
int getStrength() const;
std::string getStatus() const;
int getMaxHealth() const;

bool isAlive() const;
void takeDamage(int damage);
virtual void attack(Monster& target);
void applyStatusEffect(std::string effect, int duration);
void effectStatus();
void reset();
void heal(int amount);
void boostStrength(int amount);
void doubleStatusDuration();
std::string getSpecialEffect() const;
int getEffectChance() const;
int getEffectDuration() const;
int getCritChance() const;
float getCritMultiplier() const;

~Monster();

private:
std::string name;
int health;
int strength;
std::string currentStatus;
int statusDuration;
int effectChance;
int effectDuration;  
std::string specialEffect;
int maxHealth;
int critChance;
float critMultiplier;


};
