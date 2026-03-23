#include "Character.h"



Character::Character(string name) : name(name) {}

string Character::getName() const 
{
    return name;
}

vector<Monster*> Character::getMonsters() const 
{
    return monsters;
}

int Character::getMonsterCount() const 
{
    return monsters.size();
}

bool Character::hasMonsters() const 
{
    return !monsters.empty();
}

void Character::addMonster(Monster* monster) 
{
    monsters.push_back(monster);
    if (monsters.size() > 4) 
    {
        cout << "You can only have 4 monsters. Which one of your monsters do you want to swap out? " << endl;
        for (int i = 0; i < monsters.size(); i++) 
        {
            cout << i + 1 << ". " << monsters[i]->getName() << " (Health: " << monsters[i]->getHealth() << ", Strength: " << monsters[i]->getStrength() << ")" << endl;
        }
        int choice;
        cin >> choice;
        if (choice >= 1 && choice <= monsters.size()) 
        {
            swapMonster(choice - 1, monster);
        }
        else 
        {
            cout << "Invalid choice. The new monster will not be added." << endl;
            delete monster; 
        }
        
    }
}

void Character::swapMonster(int index, Monster* newMonster) 
{
    if (index >= 0 && index < monsters.size()) 
    {
        delete monsters[index]; // Clean up the old monster
        monsters[index] = newMonster;
    }
}

void Character::removeDefeatedMonsters() 
{
    for (auto it = monsters.begin(); it != monsters.end();) 
    {
        if (!(*it)->isAlive()) 
        {
            delete *it; // Clean up the defeated monster
            it = monsters.erase(it); // Remove from vector
        } 
        else 
        {
            ++it;
        }
    }
}

Character::~Character() 
{
    for (Monster* monster : monsters) 
    {
        delete monster; // Clean up all monsters
    }
}