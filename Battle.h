#pragma once


#include "Character.h"
#include "Enemy.h"
#include "Monster.h"
#include <vector>

Monster* chooseMonster(Character* player);
void clearScreen();
void showBattleStatus(Monster* playerMonster, Monster* enemyMonster);
void offerMonster(Character* player, const std::vector<Monster*>& defeatedMonsters);
bool battle(Character* player, Enemy* enemy);

