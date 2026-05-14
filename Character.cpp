#include "Character.h"

using namespace std;

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
            it = monsters.erase(it); // Remove from vector
        } 
        else 
        {
            ++it;
        }
    }
}


void Character::removeMonster(int index) 
{
    if (index >= 0 && index < monsters.size()) 
    {
        delete monsters[index]; // Clean up the monster
        monsters.erase(monsters.begin() + index); // Remove from vector
    }
}

void Character::addItem(Item* item)
{
    if (inventory.size() < 5)
        inventory.push_back(item);
    else
        cout << "Inventory full!" << endl;
}

void Character::swapItem(int index, Item* item)
{
    if (index >= 0 && index < (int)inventory.size())
    {
        delete inventory[index];
        inventory[index] = item;
    }
}

void Character::removeItem(int index)
{
    if (index >= 0 && index < (int)inventory.size())
    {
        delete inventory[index];
        inventory.erase(inventory.begin() + index);
    }
}

vector<Item*> Character::getInventory() const
{
    return inventory;
}

int Character::getInventoryCount() const
{
    return inventory.size();
}

Character::~Character() 
{
    for (Monster* monster : monsters) 
    {
        delete monster; // Clean up all monsters
    }
    for (Item* item : inventory)
    {
        delete item; // Clean up all items
    }
}