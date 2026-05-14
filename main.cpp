#include <iostream>
#include <string>
#include "Character.h"
#include "Monster.h"
#include "Enemy.h"
#include "Area.h"
#include "GameData.h"
#include "Battle.h"
#include "EventSystem.h"
#include <vector>

using namespace std;

Character* createCharacter() 
{
    string name;
    cout << "Enter character name: ";
    cin >> name;
    
    Character* player = new Character(name);
    player->addMonster(new Monster("Wolf", 20, 5, "bleeding", 30, 2, 25, 1.5));
    player->addMonster(new Monster("Bear", 30, 10, "", 0, 0, 20, 1.5));

    cout << "Welcome " << player->getName() << "! You start with a wolf and a bear!" << endl;
    return player;
}






void adventureLoop(Character* player, const vector<Area*>& world)
{
    while (true)
    {
        cout << "--- Areas to explore ---" << endl;
        for (size_t i = 0; i < world.size(); ++i)
        {
            cout << i + 1 << ". " << world[i]->getName() << ": " << world[i]->getDescription() << endl;
        }
        cout << "Enter the number of the area you want to explore (or 0 to quit): ";
        int areaChoice;
        cin >> areaChoice;
        cin.ignore(); // So you don't have to press enter twice after choosing an area
        
        if (areaChoice == 0)
        {
            break;
        }
        if (areaChoice < 1 || areaChoice > world.size())
        {
            cout << "Invalid choice. Please try again." << endl;
        }

        Area* area = world[areaChoice - 1];
        cout << "You enter the " << area->getName() << ". " << area->getDescription() << endl;
        EventType event = rollEvent();
        handleEvent(player, event);

        vector<Enemy*> enemies = area->getEnemies();
        if (enemies.empty())
        {
            cout << "This area is peaceful. No enemies to fight." << endl;
            continue;
        }

        cout << "Enemies in this area:" << endl;
        for (size_t i = 0; i < enemies.size(); ++i)
        {
            cout << i + 1 << ". " << enemies[i]->getName() << endl;
        }
        cout << "Enter the number of the enemy you want to fight (or 0 to go back): ";
        int enemyChoice;
        cin >> enemyChoice;
        cin.ignore(); // So you don't have to press enter twice after choosing an enemy

        if (enemyChoice == 0)
        {
            continue;
        }

        if (enemyChoice < 1 || enemyChoice > enemies.size())
        {
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }

        Enemy* enemy = enemies[enemyChoice - 1];
        bool playerWon = battle(player, enemy);
        if (playerWon)
        {
            area->removeEnemy(enemy); // Remove defeated enemy from the area
            delete enemy; 

            EventType event = rollEvent();
            handleEvent(player, event);
        }

        if (!player->hasMonsters()) 
        {
            cout << "Game Over! You have been defeated. Return to the main menu." << endl;
            return;
        }

    }
}

void mainMenu(Character* player, const vector<Area*>& world)
{
    while (true)
    {
    cout << "--- Main Menu ---" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Quit" << endl;
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    cin.ignore(); // To consume the newline character after the choice input
    
    if (choice == 1)
    {
        delete player; // Clean up previous character if it exists
        player = createCharacter();
        adventureLoop(player, world);
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
    srand(time(0)); // Ensures different random outcomes each time the game is played

    cout << "Welcome to the Monster Battle Game!" << endl;
    vector<Area*> world = setupWorld(); // Set up the game world before main menu
    Character* player = nullptr;  // Use nullptr for so the changes won't be lost
    mainMenu(player, world);
    
    for (Area* area : world) 
    {
        delete area; // Clean up areas
    }
    


    return 0;
}
