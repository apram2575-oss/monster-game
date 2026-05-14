#pragma once
#include <vector> 
#include "Area.h"
#include "Cave.h"
#include "Character.h"



std::vector<Area*> setupWorld();
Cave* generateCave(Character* player, int defeatedEnemyCount);