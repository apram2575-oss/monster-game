#include "Battle.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Monster* chooseMonster(Character* player)
{
    cout << "Choose a monster to battle with:" << endl; 
    for (size_t i = 0; i < player->getMonsters().size(); ++i) 
    {
        cout << i + 1 << ". " << player->getMonsters()[i]->getName() << " (HP: " << player->getMonsters()[i]->getHealth() << ", STR: " << player->getMonsters()[i]->getStrength() << ")" << endl;
    }
    int choice;
    cin >> choice;
    cin.ignore(); // Consume newline after integer input

    if (choice > 0 && choice <= player->getMonsters().size()) 
    {
        return player->getMonsters()[choice - 1];
    }
    return nullptr;
}

void playerTurn(Character* player, Monster* playerMonster, Monster* enemyMonster)
{
    cout << "\nYour turn!" << endl;
    cout << "1. Attack" << endl;
    cout << "2. Use item" << endl;
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1)
    {
        playerMonster->attack(*enemyMonster);
    }
    else if (choice == 2)
    {
        if (player->getInventoryCount() == 0)
        {
            cout << "No items! Attacking instead." << endl;
            playerMonster->attack(*enemyMonster);
            return;
        }

        // show inventory
        vector<Item*> inv = player->getInventory();
        for (size_t i = 0; i < inv.size(); i++)
        {
            cout << i + 1 << ". " << inv[i]->getName() 
                 << " - " << inv[i]->getDescription() << endl;
        }
        cout << "0. Cancel" << endl;

        int itemChoice;
        cin >> itemChoice;
        cin.ignore();

        if (itemChoice < 1 || itemChoice > (int)inv.size())
        {
            cout << "Cancelled! Attacking instead." << endl;
            playerMonster->attack(*enemyMonster);
            return;
        }

        Item* item = inv[itemChoice - 1];

        // determine target based on item
        if (item->getTarget() == "enemy")
            item->use(enemyMonster);
        else
            item->use(playerMonster);

        // remove if single use
        if (!item->isReusable())
            player->removeItem(itemChoice - 1);
    }
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
    cin.ignore(); // Consume newline after integer input

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
            cin.ignore(); // Consume newline after char input
            
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

    vector<Monster*> aliveMonsters; // Gets a random alive monster from the enemy
    for (Monster* m : enemy->getMonsters())
    {
        if (m->isAlive())
        {
            aliveMonsters.push_back(m);
        }
    }
    Monster* enemyMonster = aliveMonsters[rand() % aliveMonsters.size()];


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
        if (currentAttacker == playerMonster)
        {
            playerTurn(player, playerMonster, enemyMonster);
        }
        else
        {
            enemyMonster->attack(*playerMonster);
        }

        showBattleStatus(playerMonster, enemyMonster);


        cout << "DEBUG " << playerMonster->getName() 
        << " HP before effectStatus: " << playerMonster->getHealth() 
        << " status: " << playerMonster->getStatus() << endl;
        playerMonster->effectStatus();
        cout << "DEBUG " << playerMonster->getName() 
        << " HP after effectStatus: " << playerMonster->getHealth() << endl;


        cout << "DEBUG " << enemyMonster->getName() 
        << " HP before effectStatus: " << enemyMonster->getHealth() 
        << " status: " << enemyMonster->getStatus() << endl;
        enemyMonster->effectStatus();
        cout << "DEBUG " << enemyMonster->getName() 
        << " HP after effectStatus: " << enemyMonster->getHealth() << endl;


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

bool caveBattle(Character* player, Monster* caveMonster)
{
    vector<Monster*> defeatedMonsters;

    cout << "\n=== CAVE BATTLE ===" << endl;

    while (player->hasMonsters() && caveMonster->isAlive())
    {
        Monster* playerMonster = chooseMonster(player);

        bool playerFirst = rand() % 2 == 0;
        if (playerFirst)
            cout << playerMonster->getName() << " goes first!" << endl;
        else
            cout << caveMonster->getName() << " goes first!" << endl;

        Monster* attacker = playerFirst ? playerMonster : caveMonster;
        Monster* defender = playerFirst ? caveMonster : playerMonster;

        while (playerMonster->isAlive() && caveMonster->isAlive())
        {
            if (attacker == playerMonster)
                playerTurn(player, playerMonster, caveMonster);
            else
                caveMonster->attack(*playerMonster);

            showBattleStatus(playerMonster, caveMonster);

            playerMonster->effectStatus();
            caveMonster->effectStatus();

            if (!playerMonster->isAlive() || !caveMonster->isAlive())
                break;

            swap(attacker, defender);
        }

        if (!playerMonster->isAlive())
        {
            cout << "Your " << playerMonster->getName() 
                 << " was defeated!" << endl;
            player->removeDefeatedMonsters();
        }
    }

    if (caveMonster->isAlive())
        return false;

    cout << caveMonster->getName() << " was defeated!" << endl;
    return true;
}
