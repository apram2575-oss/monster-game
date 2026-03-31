#include <iostream>
#include <string>
#include "Character.h"
#include "Monster.h"
#include "Enemy.h"
#include "Area.h"
#include "GameData.h"
#include <vector>

using namespace std;

Character* createCharacter() 
{
    string name;
    cout << "Enter character name: ";
    cin >> name;
    
    Character* player = new Character(name);
    player->addMonster(new Monster("Wolf", 20, 5));
    player->addMonster(new Monster("Bear", 30, 10));

    cout << "Welcome " << player->getName() << "! You start with a wolf and a bear!" << endl;
    return player;
}



Monster* chooseMonster(Character* player) 
{
    cout << "Choose a monster to battle with:" << endl;
    for (size_t i = 0; i < player->getMonsters().size(); ++i) 
    {
        cout << i + 1 << ". " << player->getMonsters()[i]->getName() << " (HP: " << player->getMonsters()[i]->getHealth() << ", STR: " << player->getMonsters()[i]->getStrength() << ")" << endl;
    }
    int choice;
    cin >> choice;
    if (choice > 0 && choice <= player->getMonsters().size()) 
    {
        return player->getMonsters()[choice - 1];
    }
    return nullptr;
}

void clearScreen()
{
    cout << "\033[2J\033[1;1H";  // ANSI escape code to clear terminal
}

void showBattleStatus(Monster* playerMonster, Monster* enemyMonster) 
{
    clearScreen();
    cout << "\n========================================" << endl;
    cout << "  YOUR MONSTER" << endl;
    cout << "  " << playerMonster->getName() 
         << " | HP: " << playerMonster->getHealth() 
         << "/" << playerMonster->getMaxHealth()
         << " | STR: " << playerMonster->getStrength();
    if (playerMonster->getStatus() != "")
    {
        cout << " | Status: " << playerMonster->getStatus();
    }
    cout << "\n----------------------------------------" << endl;
    cout << "  ENEMY MONSTER" << endl;
    cout << "  " << enemyMonster->getName() 
         << " | HP: " << enemyMonster->getHealth()
         << "/" << enemyMonster->getMaxHealth()
         << " | STR: " << enemyMonster->getStrength();
    if (enemyMonster->getStatus() != "")
    {
        cout << " | Status: " << enemyMonster->getStatus();
    }
    cout << "\n========================================\n" << endl;
}


void offerMonster(Character* player, const vector<Monster*>& defeatedMonsters) 
{
 

    cout << "You have the opportunity to add a defeated monster to your team!" << endl;
    
    for (size_t i = 0; i < defeatedMonsters.size(); ++i) 
    {
        cout << i + 1 << ". " << defeatedMonsters[i]->getName() << " (HP: " << defeatedMonsters[i]->getHealth() << ", STR: " << defeatedMonsters[i]->getStrength() << ")" << endl;
    }
    
    cout << "Enter the number of the monster you want to add (or 0 to skip): ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= defeatedMonsters.size()) 
    {
        Monster* chosenMonster = defeatedMonsters[choice - 1];
        
        // Check if the player's party is full
        if (player->getMonsters().size() >= 4) 
        {
            cout << "\n Your party is full! You can only carry 4 monsters." << endl;
            cout << "Do you want to swap one of your current monsters for " << chosenMonster->getName() << "? (y/n): ";
            char swapConfirm;
            cin >> swapConfirm;
            
            if (swapConfirm == 'y' || swapConfirm == 'Y') 
            {
                cout << "\nChoose a monster to swap:" << endl;
                for (size_t i = 0; i < player->getMonsters().size(); ++i) 
                {
                    cout << i + 1 << ". " << player->getMonsters()[i]->getName() << " (HP: " << player->getMonsters()[i]->getHealth() << ")" << endl;
                }
                
                int swapChoice;
                cout << "Enter the number to swap (or 0 to cancel): ";
                cin >> swapChoice;
                
                if (swapChoice > 0 && swapChoice <= player->getMonsters().size()) 
                {
                    int indexToRelease = swapChoice - 1;
                    cout << "Goodbye, " << player->getMonsters()[indexToRelease]->getName() << "!" << endl;
                    
                    // Remove the old monster and add the new one
                    player->removeMonster(indexToRelease);
                    chosenMonster->reset(); // Heal and reset status effects before adding
                    player->addMonster(new Monster(*chosenMonster));
                    
                    cout << chosenMonster->getName() << " has joined your team!" << endl;
                }
                else
                {
                    cout << "Swap canceled. " << chosenMonster->getName() << " was not added." << endl;
                }
            }
            else 
            {
                cout << chosenMonster->getName() << " was left behind." << endl;
            }
        }
        else 
        {
            // Party isn't full, add normally
            chosenMonster->reset(); // Heal and reset status effects before adding
            player->addMonster(new Monster(*chosenMonster)); 
            cout << chosenMonster->getName() << " has joined your team!" << endl;
        }
    } 
    else 
    {
        cout << "Skipped. No monster added." << endl;
    }
}

bool battle(Character* player, Enemy* enemy) 
{

    vector<Monster*> defeatedMonsters; // To keep track of defeated monsters
    cout << "A battle begins between " << player->getName() << " and " << enemy->getName() << "!" << endl;

    while (player->hasMonsters() && enemy->hasMonsters())
    {
    Monster* playerMonster = chooseMonster(player); // Player picks their monster

   Monster* enemyMonster = enemy->getMonsters()[0]; // Enemy starts with their first monster

   bool whoStarts = rand() % 2; // Randomly decide who starts
   if (whoStarts == 0)
   {
        cout << player->getName() << " starts the battle!" << endl;
   }
    else
    {
          cout << enemy->getName() << " starts the battle!" << endl;
    }
    

    

    Monster* currentAttacker = whoStarts ? playerMonster : enemyMonster;
    Monster* currentDefender = whoStarts ? enemyMonster : playerMonster;

    while (playerMonster->isAlive() && enemyMonster->isAlive()) 
    {
        currentAttacker->attack(*currentDefender);

        showBattleStatus(playerMonster, enemyMonster);

        playerMonster->effectStatus();
        enemyMonster->effectStatus();

        if (!currentDefender->isAlive()) 
        {
            break;
        }

        swap(currentAttacker, currentDefender);
       
    }

    if (!enemyMonster->isAlive()) 
    {
        cout << enemyMonster->getName() << " has been defeated!" << endl;
        defeatedMonsters.push_back(enemyMonster); // Add to defeated monsters
        enemy->removeDefeatedMonsters();
        if (enemy->hasMonsters()) 
        {
            cout << enemy->getName() << " sends out " << enemy->getMonsters()[0]->getName() << "!" << endl;
        }
    }

    if (!playerMonster->isAlive()) 
    {
        cout << playerMonster->getName() << " has been defeated!" << endl;
        player->removeDefeatedMonsters();
    
        
    }
}

    if (player->hasMonsters()) 
    {
        cout << player->getName() << " wins the battle!" << endl;
        for (Monster* m : player->getMonsters()) 
        {
            m->reset(); // Heal and reset status effects for surviving monsters
        }

        offerMonster(player, defeatedMonsters); // Offer the player a chance to add a defeated monster
        for (Monster* m : defeatedMonsters) 
        {
            delete m; // Clean up defeated monsters
        }
        return true;
    } 
    else 
    {
        cout << enemy->getName() << " wins the battle!" << endl;
        for (Monster* m : defeatedMonsters) 
        {
            delete m; // Clean up defeated monsters
        }
        return false;
    }

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
