// EventSystem.cpp
#include "EventSystem.h"
#include "Monster.h"
#include "Enemy.h"
#include "Battle.h"
#include <cstdlib>
#include <iostream>
using namespace std;

EventType rollEvent()
{
    int roll = rand() % 100 + 1;

    if (roll <= 20) return EventType::NOTHING;
    else if (roll <= 30) return EventType::RANDOM_ENEMY;
    else if (roll <= 50) return EventType::SALESMAN;
    else if (roll <= 70) return EventType::ABANDONED_MONSTER;
    else if (roll <= 85) return EventType::HEALING_SPRING;
    else return EventType::MYSTERIOUS_STRANGER;
}

void handleEvent(Character* player, EventType event)
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
            // battle happens here - but battle needs to be accessible
            // we'll discuss this below
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
            // pick a random monster to offer
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
            if (choice == 1)
                offerMonster(player, {found});
            
            // clean up unchosen monsters
            for (Monster* m : wildMonsters)
                if (m != found || choice != 1)
                    delete m;
            break;
        }

        case EventType::MYSTERIOUS_STRANGER:
        {
            cout << "\nA mysterious stranger appears!" << endl;

            
        }

        case EventType::HEALING_SPRING:
        {
            cout << "\nYou discover a healing spring!" << endl;
            cout << "All your monsters are fully healed!" << endl;
            for (Monster* m : player->getMonsters())
                m->reset();
            break;
        }
    }
}