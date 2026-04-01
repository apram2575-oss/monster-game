#include "Monster.h"
#include <iostream>



Monster::Monster(string name, int health, int strength, string specialEffect, int effectChance, int effectDuration, int critChance, float critMultiplier): name(name), health(health), maxHealth(health), strength(strength), specialEffect(specialEffect), effectChance(effectChance), effectDuration(effectDuration), currentStatus(""), statusDuration(0), critChance(critChance), critMultiplier(critMultiplier) {}

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

int Monster::getMaxHealth() const 
{
    return maxHealth;
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

void Monster::applyStatusEffect(string effect, int duration) 
{
    currentStatus = effect;
    statusDuration = duration;
}

string Monster::getStatus() const 
{
    return currentStatus;
}

void Monster::attack(Monster& target) 
{
    if (isAlive()) 
    {
        target.takeDamage(strength);
        cout << name << " attacks " << target.getName() << " for " << strength << " damage!" << endl;

        int damage;
        int critRoll = rand() % 100;
        if (critRoll < critChance)
        {
            damage = static_cast<int>(strength * critMultiplier);
            cout << "Critical hit! " << name << " deals an additional " << damage - strength << " damage!" << endl;
        }
        target.takeDamage(damage);

        if (specialEffect != "" && 0 < effectChance) 
        {
           int roll = rand() % 100;
           if (roll < effectChance) 
           {
            target.applyStatusEffect(specialEffect, effectDuration);
            cout << name << " applies " << specialEffect << " to " << target.getName() << " for " << effectDuration << " turns!" << endl;
           }
        }
    }
}

void Monster::effectStatus() 
{
    if (currentStatus == "poison" && statusDuration > 0) 
    {
        health -= 5; 
        cout << name << " takes 5 poison damage!" << endl;
        statusDuration--;
        if (statusDuration == 0) 
        {
            currentStatus = "";
            cout << name << " is no longer poisoned!" << endl;
        }
    }
    else if (currentStatus == "curse" && statusDuration > 0) 
    {
        strength -= 5; 
        cout << name << " loses 2 strength due to the curse!" << endl;
        statusDuration--;
        int roll = rand() % 100;
        if (roll < 20) 
        {
            takeDamage(strength);
            cout << name << " accidentally hurts itself!" << endl;
        }
        if (statusDuration == 0) 
        {
            currentStatus = "";
            cout << name << " is no longer cursed!" << endl;
        }
    }

    else if (currentStatus == "burn" && statusDuration > 0) 
    {
        health -= 10; 
        cout << name << " takes 10 burn damage!" << endl;
        statusDuration--;
        if (statusDuration == 0) 
        {
            currentStatus = "";
            cout << name << " is no longer burning!" << endl;
        }
    }
    else if (currentStatus == "bleeding" && statusDuration > 0) 
    {
        health -= 7; 
        cout << name << " takes 7 bleeding damage!" << endl;
        statusDuration--;
        if (statusDuration == 0) 
        {
            currentStatus = "";
            cout << name << " is no longer bleeding!" << endl;
        }
    }
}

void Monster::reset()
{
    health = maxHealth;
    currentStatus = "";
    statusDuration = 0;
}

Monster::~Monster() {}