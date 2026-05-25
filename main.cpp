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
#include "DataBase.h"

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






void adventureLoop(Character* player, vector<Area*>& world, Stats& stats, Database& db, int characterId)
{
    int defeatedEnemyCount = 0;
    Cave* currentCave = generateCave(player, defeatedEnemyCount);
    bool strangerAppeared = false;

 
   

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
            bool hasItems = player->getInventoryCount() > 0;
            EventType event = rollEvent(strangerAppeared, hasItems);
            handleEvent(player, event, strangerAppeared, stats);

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
            bool playerWon = battle(player, enemy, stats);
            vector<pair<string,string>> defeatedEnemies; // vector to save defeated enemies for database saving

            if (playerWon)
            {
                area->removeEnemy(enemy);
                defeatedEnemies.push_back(make_pair(area->getName(), enemy->getName())); // when enemy gets defeated
                stats.totalMonstersDefeated++;

                cout << "DEBUG: characterId = " << characterId << endl;
                cout << "DEBUG: Saving defeated enemy " << enemy->getName() 
                     << " in " << area->getName() << endl;
               
                db.saveDefeatedEnemy(characterId, area->getName(), enemy->getName());
                

                delete enemy;
                defeatedEnemyCount++;

                // regenerate cave after each defeated enemy
                delete currentCave;
                currentCave = generateCave(player, defeatedEnemyCount);
                cout << "\nA new cave has appeared!" << endl;

                // event after winning
                bool hasItems = player->getInventoryCount() > 0;
                EventType event = rollEvent(strangerAppeared, hasItems);
                handleEvent(player, event, strangerAppeared, stats);
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
                bool playerWon = caveBattle(player, caveMonster, stats);

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

void mainMenu(Character*& player, vector<Area*>& world,
    Stats& stats, Database& db, int& characterId)
    {
        while (true)
        {
        cout << "\n--- Main Menu ---" << endl;
        cout << "1. New Game" << endl;
        cout << "2. Start Adventure" << endl;
        cout << "3. Save Game" << endl;
        cout << "4. View Stats" << endl;
        cout << "5. Quit" << endl;
        cout << "Choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
        delete player;
        player = createCharacter();
        stats = Stats();  // reset stats for new game
        characterId = -1;
        }
        else if (choice == 2)
        {
        if (player == nullptr)
        {
            cout << "Create a character first!" << endl;
            continue;
        }
        adventureLoop(player, world, stats, db, characterId);
        }
        else if (choice == 3)
        {
        if (player == nullptr)
        {
            cout << "No character to save!" << endl;
            continue;
        }
        db.saveCharacter(player, stats, world);
        // update characterId if first save
        if (characterId == -1)
        {
            vector<HeroSummary> heroes = db.getAllHeroes();
            for (auto& h : heroes)
                if (h.name == player->getName())
                    characterId = h.id;
        }
        }
        else if (choice == 4)
        {
        if (characterId == -1)
            cout << "Save your game first to see stats!" << endl;
        else
            db.printStats(characterId);
        }
        else if (choice == 5)
        {
        cout << "Are you sure? (y/n): ";
        char confirm;
        cin >> confirm;
        cin.ignore();
        if (confirm == 'y' || confirm == 'Y')
        {
            delete player;
            return;
        }
        }
    }
}


int main()
{
    srand(time(0));
    
    cout << "Welcome to the Monster Battle Game!" << endl;

    Database db("monsters.db");
    vector<Area*> world = setupWorld();
    Character* player = nullptr;
    Stats stats;
    int characterId = -1;


    while (true)
    {
        vector<HeroSummary> heroes = db.getAllHeroes();

        cout << "\n=== SAVED HEROES ===" << endl;

        if (heroes.empty())
        {
            cout << "No saved heroes. Starting new game!" << endl;
            break;
        }

        for (size_t i = 0; i < heroes.size(); i++)
        {
            cout << i + 1 << ". " << heroes[i].name
                 << " | Monsters: " << heroes[i].monsterCount
                 << " | Defeated: " << heroes[i].totalMonstersDefeated
                 << " | Items: " << heroes[i].itemCount
                 << endl;
        }
        cout << heroes.size() + 1 << ". New Game" << endl;
        cout << heroes.size() + 2 << ". Delete a hero" << endl;
        cout << "Choice: ";

        int heroChoice;
        cin >> heroChoice;
        cin.ignore();

        if (heroChoice > 0 && heroChoice <= (int)heroes.size())
        {
            // load existing hero
            characterId = heroes[heroChoice - 1].id;
            player = db.loadCharacter(characterId, stats, world);
            break;  // valid choice, exit loop
        }
        else if (heroChoice == (int)heroes.size() + 1)
        {
            // new game
            break;  // exit loop, player creates character in main menu
        }
        else if (heroChoice == (int)heroes.size() + 2)
        {
            // delete a hero
            cout << "\nChoose a hero to delete:" << endl;
            for (size_t i = 0; i < heroes.size(); i++)
                cout << i + 1 << ". " << heroes[i].name << endl;
            cout << "0. Cancel" << endl;
            cout << "Choice: ";

            int deleteChoice;
            cin >> deleteChoice;
            cin.ignore();

            if (deleteChoice > 0 && deleteChoice <= (int)heroes.size())
            {
                cout << "Are you sure you want to delete "
                     << heroes[deleteChoice - 1].name << "? (y/n): ";
                char confirm;
                cin >> confirm;
                cin.ignore();

                if (confirm == 'y' || confirm == 'Y')
                {
                    db.deleteHero(heroes[deleteChoice - 1].id);
                    cout << heroes[deleteChoice - 1].name
                         << " has been deleted!" << endl;
                }
                else
                {
                    cout << "Deletion cancelled." << endl;
                }
            }
            // loop continues, shows updated hero list
        }
        else
        {
            cout << "Invalid choice!" << endl;
            // loop continues
        }
    }

    mainMenu(player, world, stats, db, characterId);

    for (Area* area : world)
        delete area;

    return 0;
}