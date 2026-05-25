
#pragma once

#include "Character.h"
#include "Monster.h"
#include "Enemy.h"
#include "DataBase.h"

enum class EventType 
{
    NOTHING,
    RANDOM_ENEMY,
    SALESMAN,
    ABANDONED_MONSTER,
    MAGICAL_SPRING,
    MYSTERIOUS_STRANGER
};

EventType rollEvent(bool strangerAppeared, bool hasItems);
void handleEvent(Character* player, EventType event, bool& strangerAppeared, Stats& stats);

