
#pragma once

#include "Character.h"
#include "Monster.h"
#include "Enemy.h"

enum class EventType 
{
    NOTHING,
    RANDOM_ENEMY,
    SALESMAN,
    ABANDONED_MONSTER,
    HEALING_SPRING,
    MYSTERIOUS_STRANGER
};

EventType rollEvent(bool strangerAppeared, bool hasItems);
void handleEvent(Character* player, EventType event, bool& strangerAppeared);

