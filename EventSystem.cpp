#include "EventSystem.h"
#include "Battle.h"
#include "Item.h"
#include <iostream>
#include <cstdlib>
using namespace std;

EventType rollEvent(bool strangerAppeared, bool hasItems)
{
    // stranger can only appear if not seen before and player has items
    if (!strangerAppeared && hasItems)
    {
        int strangerRoll = rand() % 100;
        if (strangerRoll < 30)  // 30% chance to appear
            return EventType::MYSTERIOUS_STRANGER;
    }

    int roll = rand() % 100;
    if (roll < 30)       return EventType::NOTHING;
    else if (roll < 50)  return EventType::RANDOM_ENEMY;
    else if (roll < 65)  return EventType::SALESMAN;
    else if (roll < 80)  return EventType::ABANDONED_MONSTER;
    else                 return EventType::HEALING_SPRING;
}

void handleEvent(Character* player, EventType event, bool& strangerAppeared)
{
    switch (event)
    {
        case EventType::NOTHING:
            break;

        case EventType::RANDOM_ENEMY:
        {
            cout << "\nA wild enemy appears!" << endl;
            Enemy* randomEnemy = new Enemy("Wandering Beast", "Wilderness");
            randomEnemy->addMonster(new Monster("Wild Wolf", 30, 8));
            battle(player, randomEnemy);
            delete randomEnemy;
            break;
        }

        case EventType::SALESMAN:
        {
            cout << "\nA travelling salesman appears!" << endl;
            cout << "He offers to heal all your monsters for free!" << endl;
            for (Monster* m : player->getMonsters())
                m->reset();
            cout << "All your monsters have been healed!" << endl;
            break;
        }

        case EventType::ABANDONED_MONSTER:
        {
            cout << "\nYou find an abandoned monster!" << endl;
            vector<Monster*> wildMonsters = {
                new Monster("Wild Boar", 25, 7),
                new Monster("Stray Cat", 15, 5, "poison", 20, 2),
                new Monster("Lost Pup", 20, 6)
            };
            int pick = rand() % wildMonsters.size();
            Monster* found = wildMonsters[pick];

            cout << "You found a " << found->getName() << "!" << endl;
            cout << "1. Add to team" << endl;
            cout << "2. Leave it" << endl;

            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 1)
            {
                vector<Monster*> toOffer = {found};
                offerMonster(player, toOffer);
            }

            for (Monster* m : wildMonsters)
                if (m != found)
                    delete m;
            break;
        }

        case EventType::HEALING_SPRING:
        {
            cout << "\nYou discover a healing spring!" << endl;
            cout << "All your monsters are fully healed!" << endl;
            for (Monster* m : player->getMonsters())
                m->reset();
            break;
        }

        case EventType::MYSTERIOUS_STRANGER:
        {
            strangerAppeared = true;  // mark as appeared, won't show again

            cout << "\n??? : A mysterious stranger appears..." << endl;
            cout << "??? : Greetings, traveler. I am in need of help, can you spare an item, please?" << endl;
            cout << "You notice there is something strange about this person..." << endl;
            cout << "She is not alone. A black cat follows her every step." << endl;
            cout << "Player: Who are you and where did you come from?" << endl;
            cout << "???: I am a traveler like yourself, seeking refuge from the wilderness." << endl;



            // show player inventory
            vector<Item*> inv = player->getInventory();
            cout << "\nYour inventory:" << endl;
            for (int i = 0; i < (int)inv.size(); i++)
            {
                cout << i + 1 << ". " << inv[i]->getName()
                     << " - " << inv[i]->getDescription() << endl;
            }
            cout << "0. I refuse to give you anything!" << endl;
            cout << "Choice: ";

            int choice;
            cin >> choice;
            cin.ignore();

            if (choice > 0 && choice <= (int)inv.size())
            {
                // player gives an item
                cout << "\nMay : Thank you for your kindness. In return, I offer you this gift." << endl;
                player->removeItem(choice - 1);

                // give powerful reward item
                vector<Item*> powerfulRewards = {
                    new Item("Elixir of Life",
                             "Fully restores all your monsters HP",
                             "heal", "friendly", 9999, "", true),  // reusable once

                    new Item("Void Orb",
                             "Deals 80 damage to enemy monster",
                             "damage", "enemy", 80),

                    new Item("Ancient Tonic",
                             "Permanently boosts your monster's strength by 50%",
                             "boost", "friendly", 50),

                    new Item("Chaos Scroll",
                             "Applies poison, burn and curse to enemy",
                             "status", "enemy", 0, "poison")
                };

                Item* reward = powerfulRewards[rand() % powerfulRewards.size()];

                // clean up unchosen rewards
                for (Item* r : powerfulRewards)
                    if (r != reward)
                        delete r;

                cout << "You received: " << reward->getName()
                     << " - " << reward->getDescription() << endl;

                if (player->getInventoryCount() < 5)
                {
                    player->addItem(reward);
                    cout << reward->getName() << " added to inventory!" << endl;
                }
                else
                {
                    cout << "Inventory full! Choose an item to swap:" << endl;
                    vector<Item*> currentInv = player->getInventory();
                    for (int i = 0; i < (int)currentInv.size(); i++)
                        cout << i + 1 << ". " << currentInv[i]->getName() << endl;
                    cout << "0. Skip" << endl;

                    int swapChoice;
                    cin >> swapChoice;
                    cin.ignore();

                    if (swapChoice > 0 && swapChoice <= (int)currentInv.size())
                        player->swapItem(swapChoice - 1, reward);
                    else
                        delete reward;
                }

                cout << "\nMay : Until we meet again..." << endl;
            }
            else
            {
                // player refuses - battle!
                cout << "\nMay: Your greed has blinded you. I will show you the error of your ways!" << endl;

                // create the black cat as a standalone monster for cave battle
                Monster* blackCat = new Monster("May's Black Cat", 60, 20, "curse", 60, 3, 30, 2.5f);

                cout << "You notice the cat has a mysterious aura and is coming right at you!" << endl;
                bool playerWon = caveBattle(player, blackCat);
                bool catResurrected = false;


                if (playerWon)
                {
                    // 50% chance stranger resurrects the cat
                    int resurrectRoll = rand() % 100;
                    if (resurrectRoll < 50)
                    {
                        cout << "\nMay: Impressive... but we are not done yet!" << endl;
                        cout << "May: Rise!" << endl;
                        cout << "\nThe Black Cat is resurrected with full power!" << endl;

                        // reset cat but make it stronger second time
                        blackCat->reset();

                        playerWon = caveBattle(player, blackCat);

                        if (playerWon)
                        {
                            cout << "\nMay: Impossible..." << endl;
                        }
                        else
                        {
                            cout << "\nMay: As I expected. Begone." << endl;
                            delete blackCat;
                            return;  // player lost, no reward
                        }
                    }
                    else
                    {
                        cout << "\nMay: You got lucky this time..." << endl;
                    }
                }
                if (playerWon)
                {
                    cout << "\nThe stranger vanishes into the shadows!" << endl;

                    vector<Monster*> catReward = {blackCat};
                    offerMonster(player, catReward);

                    
                    vector<Item*> possibleRewards = {
                        new Item("Stranger's Relic",
                                "Deals 60 damage and poisons the enemy",
                                "damage", "enemy", 60, "poison"),

                        new Item("Shadow Orb",
                                "Deals 70 damage to enemy monster",
                                "damage", "enemy", 70),

                        new Item("Void Tonic",
                                "Permanently boosts your monster's strength by 40%",
                                "boost", "friendly", 40)
                    };

                    if (catResurrected)
                    {
                        possibleRewards.push_back(new Item(
                            "Resurrection Potion",
                            "Resurrects one of your defeated monsters with full HP",
                            "resurrect", "friendly", 0, "", false
                        ));

                        cout << "\nBecause the cat was resurrected, a rare item has appeared!" << endl;
                    }

                    // pick random reward
                    Item* battleReward = possibleRewards[rand() % possibleRewards.size()];

                    // clean up unchosen rewards
                    for (Item* r : possibleRewards)
                        if (r != battleReward)
                            delete r;

                    cout << "\nYou found: " << battleReward->getName()
                        << " - " << battleReward->getDescription() << endl;

                    if (player->getInventoryCount() < 5)
                    {
                        player->addItem(battleReward);
                        cout << battleReward->getName() << " added to inventory!" << endl;
                    }
                    else
                    {
                        cout << "Inventory full! Choose an item to swap:" << endl;
                        vector<Item*> currentInv = player->getInventory();
                        for (int i = 0; i < (int)currentInv.size(); i++)
                            cout << i + 1 << ". " << currentInv[i]->getName() << endl;
                        cout << "0. Skip" << endl;

                        int swapChoice;
                        cin >> swapChoice;
                        cin.ignore();

                        if (swapChoice > 0 && swapChoice <= (int)currentInv.size())
                            player->swapItem(swapChoice - 1, battleReward);
                        else
                            delete battleReward;
                    }
                }
                else
                {
                    cout << "\n??? : Pathetic. Don't waste my time." << endl;
                    delete blackCat;
                }
            }
            break;
        }
    }
}