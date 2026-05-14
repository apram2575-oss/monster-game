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
#include "Item.h"
#include "Cave.h"

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






void adventureLoop(Character* player, vector<Area*>& world)
{
    int defeatedEnemyCount = 0;
    Cave* currentCave = generateCave(player, defeatedEnemyCount);

    while (true)
    {
        cout << "\n--- Adventure Menu ---" << endl;
        cout << "1. Explore areas" << endl;
        cout << "2. Enter cave (" << currentCave->getName() << ")" << endl;
        cout << "0. Return to main menu" << endl;

        int menuChoice;
        cin >> menuChoice;
        cin.ignore();

        if (menuChoice == 0) break;

        // Areas
        else if (menuChoice == 1)
        {
            cout << "\n--- Areas to explore ---" << endl;
            for (size_t i = 0; i < world.size(); ++i)
            {
                cout << i + 1 << ". " << world[i]->getName() 
                     << ": " << world[i]->getDescription() << endl;
            }
            cout << "0. Go back" << endl;
            cout << "Choice: ";

            int areaChoice;
            cin >> areaChoice;
            cin.ignore();

            if (areaChoice == 0) continue;

            if (areaChoice < 1 || areaChoice > (int)world.size())
            {
                cout << "Invalid choice. Please try again." << endl;
                continue;
            }

            Area* area = world[areaChoice - 1];
            cout << "\nYou enter " << area->getName() 
                 << ". " << area->getDescription() << endl;

            // event on entering area
            EventType event = rollEvent();
            handleEvent(player, event);

            vector<Enemy*> enemies = area->getEnemies();
            if (enemies.empty())
            {
                cout << "This area is cleared. No enemies remain." << endl;
                continue;
            }

            cout << "\nEnemies in this area:" << endl;
            for (size_t i = 0; i < enemies.size(); ++i)
                cout << i + 1 << ". " << enemies[i]->getName() << endl;
            cout << "0. Go back" << endl;
            cout << "Choice: ";

            int enemyChoice;
            cin >> enemyChoice;
            cin.ignore();

            if (enemyChoice == 0) continue;

            if (enemyChoice < 1 || enemyChoice > (int)enemies.size())
            {
                cout << "Invalid choice. Please try again." << endl;
                continue;
            }

            Enemy* enemy = enemies[enemyChoice - 1];
            bool playerWon = battle(player, enemy);

            if (playerWon)
            {
                area->removeEnemy(enemy);
                delete enemy;
                defeatedEnemyCount++;

                // regenerate cave after each defeated enemy
                delete currentCave;
                currentCave = generateCave(player, defeatedEnemyCount);
                cout << "\nA new cave has appeared!" << endl;

                // event after winning
                EventType postEvent = rollEvent();
                handleEvent(player, postEvent);
            }

            if (!player->hasMonsters())
            {
                cout << "All your monsters were defeated! Returning to main menu." << endl;
                delete currentCave;
                return;
            }
        }

        // Caves
        else if (menuChoice == 2)
        {
            cout << "\nEntering " << currentCave->getName() << "!" << endl;
            cout << currentCave->getDescription() << endl;

            while (currentCave->hasMonsters())
            {
                // get first alive monster
                Monster* caveMonster = nullptr;
                for (Monster* m : currentCave->getMonsters())
                {
                    if (m->isAlive()) 
                    { 
                        caveMonster = m; 
                        break; 
                    }
                }

                cout << "\nA " << caveMonster->getName() << " appears!" << endl;
                bool playerWon = caveBattle(player, caveMonster);

                if (!playerWon)
                {
                    cout << "You were driven out of the cave!" << endl;
                    break;
                }

                currentCave->removeDefeatedMonsters();

                if (!player->hasMonsters())
                {
                    cout << "All your monsters were defeated! Returning to main menu." << endl;
                    delete currentCave;
                    return;
                }
            }

            // cave fully cleared
            if (!currentCave->hasMonsters())
            {
                cout << "\n=== CAVE CLEARED ===" << endl;
                currentCave->clear();

                vector<Item*> rewards = currentCave->getRewardItems();
                cout << "You found " << rewards.size() << " item(s)!" << endl;

                for (Item* reward : rewards)
                {
                    cout << "\nFound: " << reward->getName()
                         << " - " << reward->getDescription() << endl;

                    if (player->getInventoryCount() < 5)
                    {
                        player->addItem(reward);
                        cout << reward->getName() << " added to inventory!" << endl;
                    }
                    else
                    {
                        cout << "Inventory full! Choose an item to swap:" << endl;
                        vector<Item*> inv = player->getInventory();
                        for (int i = 0; i < (int)inv.size(); i++)
                            cout << i + 1 << ". " << inv[i]->getName() 
                                 << " - " << inv[i]->getDescription() << endl;
                        cout << "0. Skip" << endl;

                        int swapChoice;
                        cin >> swapChoice;
                        cin.ignore();

                        if (swapChoice > 0 && swapChoice <= (int)inv.size())
                        {
                            player->swapItem(swapChoice - 1, reward);
                            cout << reward->getName() << " added to inventory!" << endl;
                        }
                        else
                            cout << "Skipped." << endl;
                    }
                }

                // generate new cave after clearing
                delete currentCave;
                currentCave = generateCave(player, defeatedEnemyCount);
                cout << "\nA new cave has been generated!" << endl;
            }
        }
        else
        {
            cout << "Invalid choice!" << endl;
        }
    }

    delete currentCave;
}

void mainMenu(Character* player, vector<Area*>& world)
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
