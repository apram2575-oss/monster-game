#include <iostream>
#include <string>
#include "Character.h"
#include "Monster.h"
using namespace std;

Character* createCharacter() 
{
    string name;
    cout << "Enter character name: ";
    cin >> name;
    
    Character* player = new Character(name);
    player->addMonster(new Monster("Wolf", 20, 5));
    player->addMonster(new Monster("Bear", 30, 10));

    cout << "Welcome " << player->getName() << " you start with a wolf and a bear!" << endl;
    return player;
}

void mainMenu(Character* player)
{
    while (true)
    {
    cout << "Welcome to the Monster Battle Game!" << endl;
    cout << "--- Main Menu ---" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Quit" << endl;
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    
    if (choice == 1)
    {
        player = createCharacter();
    }
    if (choice == 2)
    {
        cout << "Are you sure you want to quit? (y/n): ";
        char confirm;
        cin >> confirm;
        if (confirm == 'y' || confirm == 'Y')
        {
            cout << "Are you really sure you want to quit? (y/n): ";
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y')
            {
                delete player;
                return; 
            }
            else if (confirm == 'n' || confirm == 'N')
            {
                continue; 
            }
        }
        else if (confirm == 'n' || confirm == 'N')
        {
            continue; 
        }
    }

    } 
}

int main() 
{ 
    
    
    
    // Main game loop
    cout << "Welcome to the Monster Battle Game!" << endl;

    return 0;
}
