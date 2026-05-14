
#include "Item.h"
#include "Monster.h"
#include <iostream>
using namespace std; 

Item::Item(string name, string description, string effect, string target, int value, string statusEffect, bool reusable) : name(name), description(description), effect(effect), target(target), value(value), statusEffect(statusEffect), reusable(reusable) {}

Item::~Item() {}

string Item::getName() const 
{ 
    return name; 
}
string Item::getDescription() const 
{ 
    return description; 
}
string Item::getEffect() const 
{ 
    return effect; 
}
string Item::getTarget() const 
{ 
    return target; 
}
int Item::getValue() const 
{ 
    return value; 
}
string Item::getStatusEffect() const 
{ 
    return statusEffect; 
}
bool Item::isReusable() const 
{ 
    return reusable; 
}

void Item::use(Monster* target)
{
    if (effect == "heal")
    {
        target->heal(value);
        cout << name << " heals " << target->getName() 
             << " for " << value << " HP!" << endl;
    }
    else if (effect == "damage")
    {
        target->takeDamage(value);
        cout << name << " deals " << value 
             << " damage to " << target->getName() << "!" << endl;
    }
    else if (effect == "boost")
    {
        int boost = static_cast<int>(target->getStrength() * (value / 100.0f));
        target->boostStrength(boost);
        cout << name << " boosts " << target->getName() 
             << "'s strength by " << boost << "!" << endl;
    }
    else if (effect == "status")
    {
        string current = target->getStatus();
        if (current == statusEffect)
        {
            target->doubleStatusDuration();
            cout << name << " doubles " << target->getName() 
                 << "'s " << statusEffect << " duration!" << endl;
        }
        else
        {
            target->applyStatusEffect(statusEffect, 3);
            cout << name << " applies " << statusEffect 
                 << " to " << target->getName() << "!" << endl;
        }
    }
}