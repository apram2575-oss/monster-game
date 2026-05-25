#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>

#include "Character.h"
#include "Item.h"
#include "Monster.h"
#include "Area.h"

struct HeroSummary
{
    int id;
    std::string name;
    int monsterCount;
    int totalMonstersDefeated;
    int itemCount;
};

struct Stats
{
    int totalMonstersDefeated = 0;
    std::map<std::string, int> itemUsageCount;    // itemName -> times used
    std::map<std::string, int> itemKillCount;     // itemName -> monsters killed
    std::map<std::string, int> monsterUsageCount; // monsterName -> times used
};

class Database
{
private:
    sqlite3* db;
    std::string dbPath;

    bool executeSQL(const std::string& sql);
    void createTables();

public:
    Database(const std::string& path = "monsters.db");
    ~Database();

    bool isOpen() const;

    // hero save/load
    void saveCharacter(Character* player, const Stats& stats, const std::vector<Area*>& world);
    Character* loadCharacter(int id, Stats& stats, std::vector<Area*>& world);

    // get all saved heroes for selection screen
    std::vector<HeroSummary> getAllHeroes();

    // delete a saved hero
    void deleteHero(int id);

    // stats
    void saveStats(int characterId, const Stats& stats);
    Stats loadStats(int characterId);

    // display stats
    void printStats(int characterId);
    void printGlobalStats();

    void saveDefeatedEnemy(int characterId, const std::string& areaName, const std::string& enemyName);
};

