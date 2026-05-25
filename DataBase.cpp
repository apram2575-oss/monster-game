#include "DataBase.h"
#include "Enemy.h"
#include <iostream>
#include <sstream>
using namespace std;

Database::Database(const string& path) : dbPath(path), db(nullptr)
{
    int result = sqlite3_open(path.c_str(), &db);
    if (result != SQLITE_OK)
    {
        cerr << "Failed to open database: " 
             << sqlite3_errmsg(db) << endl;
        db = nullptr;
    }
    else
    {
        executeSQL("PRAGMA journal_mode=WAL;");
        executeSQL("PRAGMA synchronous=NORMAL;");
        createTables();
    }
}

Database::~Database()
{
    if (db)
        sqlite3_close(db);
}

bool Database::isOpen() const
{
    return db != nullptr;
}

bool Database::executeSQL(const string& sql)
{
    char* errMsg = nullptr;
    int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (result != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

void Database::createTables()
{
    // characters table
    executeSQL(R"(
        CREATE TABLE IF NOT EXISTS characters (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL
        );
    )");

    // monsters table
    executeSQL(R"(
        CREATE TABLE IF NOT EXISTS monsters (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            character_id INTEGER NOT NULL,
            name TEXT NOT NULL,
            hp INTEGER NOT NULL,
            maxHp INTEGER NOT NULL,
            strength INTEGER NOT NULL,
            specialEffect TEXT,
            effectChance INTEGER,
            effectDuration INTEGER,
            critChance INTEGER,
            critMultiplier REAL,
            FOREIGN KEY(character_id) REFERENCES characters(id)
        );
    )");

    // items table
    executeSQL(R"(
        CREATE TABLE IF NOT EXISTS items (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            character_id INTEGER NOT NULL,
            name TEXT NOT NULL,
            description TEXT,
            effect TEXT,
            target TEXT,
            value INTEGER,
            statusEffect TEXT,
            reusable INTEGER,
            FOREIGN KEY(character_id) REFERENCES characters(id)
        );
    )");

    // statistics table
    executeSQL(R"(
        CREATE TABLE IF NOT EXISTS statistics (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            character_id INTEGER NOT NULL,
            totalMonstersDefeated INTEGER DEFAULT 0,
            FOREIGN KEY(character_id) REFERENCES characters(id)
        );
    )");

    // item usage table
    executeSQL(R"(
        CREATE TABLE IF NOT EXISTS item_usage (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            character_id INTEGER NOT NULL,
            itemName TEXT NOT NULL,
            timesUsed INTEGER DEFAULT 0,
            monstersDefeated INTEGER DEFAULT 0,
            FOREIGN KEY(character_id) REFERENCES characters(id)
        );
    )");

    // monster usage table
    executeSQL(R"(
        CREATE TABLE IF NOT EXISTS monster_usage (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            character_id INTEGER NOT NULL,
            monsterName TEXT NOT NULL,
            timesUsed INTEGER DEFAULT 0,
            FOREIGN KEY(character_id) REFERENCES characters(id)
        );
    )");

    // defeated enemies table - tracks which enemies are gone
    executeSQL(R"(
        CREATE TABLE IF NOT EXISTS defeated_enemies (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            character_id INTEGER NOT NULL,
            areaName TEXT NOT NULL,
            enemyName TEXT NOT NULL,
            FOREIGN KEY(character_id) REFERENCES characters(id)
        );
    )");
}


void Database::saveCharacter(Character* player, const Stats& stats,
    const vector<Area*>& world)
{
if (!db)
{
cout << "DEBUG: Database not open!" << endl;
return;
}

cout << "DEBUG: Starting save for " << player->getName() << endl;

// check if character already exists
string checkSQL = "SELECT id FROM characters WHERE name = '" 
+ player->getName() + "';";

int existingId = -1;
sqlite3_stmt* stmt;
if (sqlite3_prepare_v2(db, checkSQL.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
{
if (sqlite3_step(stmt) == SQLITE_ROW)
existingId = sqlite3_column_int(stmt, 0);
sqlite3_finalize(stmt);
}

cout << "DEBUG: Existing ID = " << existingId << endl;

int characterId;

if (existingId != -1)
{
characterId = existingId;
cout << "DEBUG: Deleting old data for character " << characterId << endl;
executeSQL("DELETE FROM monsters WHERE character_id = " 
+ to_string(characterId) + ";");
executeSQL("DELETE FROM items WHERE character_id = " 
+ to_string(characterId) + ";");
executeSQL("DELETE FROM statistics WHERE character_id = " 
+ to_string(characterId) + ";");
executeSQL("DELETE FROM item_usage WHERE character_id = " 
+ to_string(characterId) + ";");
executeSQL("DELETE FROM monster_usage WHERE character_id = " 
+ to_string(characterId) + ";");
}
else
{
cout << "DEBUG: Inserting new character" << endl;
executeSQL("INSERT INTO characters (name) VALUES ('" 
+ player->getName() + "');");
characterId = sqlite3_last_insert_rowid(db);
cout << "DEBUG: New character ID = " << characterId << endl;
}

// save monsters
cout << "DEBUG: Saving monsters, count = " 
<< player->getAllMonsters().size() << endl;

for (Monster* m : player->getAllMonsters())
{
cout << "DEBUG: Saving monster: " << m->getName() << endl;
cout << "DEBUG:   health = " << m->getHealth() << endl;
cout << "DEBUG:   maxHealth = " << m->getMaxHealth() << endl;
cout << "DEBUG:   strength = " << m->getStrength() << endl;
cout << "DEBUG:   specialEffect = " << m->getSpecialEffect() << endl;
cout << "DEBUG:   effectChance = " << m->getEffectChance() << endl;
cout << "DEBUG:   effectDuration = " << m->getEffectDuration() << endl;
cout << "DEBUG:   critChance = " << m->getCritChance() << endl;
cout << "DEBUG:   critMultiplier = " << m->getCritMultiplier() << endl;

string sql = "INSERT INTO monsters (character_id, name, hp, maxHp, "
"strength, specialEffect, effectChance, effectDuration, "
"critChance, critMultiplier) VALUES ("
+ to_string(characterId) + ", '"
+ m->getName() + "', "
+ to_string(m->getHealth()) + ", "
+ to_string(m->getMaxHealth()) + ", "
+ to_string(m->getStrength()) + ", '"
+ m->getSpecialEffect() + "', "
+ to_string(m->getEffectChance()) + ", "
+ to_string(m->getEffectDuration()) + ", "
+ to_string(m->getCritChance()) + ", "
+ to_string(m->getCritMultiplier()) + ");";

cout << "DEBUG: SQL = " << sql << endl;
executeSQL(sql);
cout << "DEBUG: Monster saved successfully" << endl;
}

// save items
cout << "DEBUG: Saving items, count = " 
<< player->getInventory().size() << endl;

for (Item* i : player->getInventory())
{
cout << "DEBUG: Saving item: " << i->getName() << endl;
cout << "DEBUG:   description = " << i->getDescription() << endl;
cout << "DEBUG:   effect = " << i->getEffect() << endl;
cout << "DEBUG:   target = " << i->getTarget() << endl;
cout << "DEBUG:   value = " << i->getValue() << endl;
cout << "DEBUG:   statusEffect = " << i->getStatusEffect() << endl;
cout << "DEBUG:   reusable = " << i->isReusable() << endl;

string sql = "INSERT INTO items (character_id, name, description, "
"effect, target, value, statusEffect, reusable) VALUES ("
+ to_string(characterId) + ", '"
+ i->getName() + "', '"
+ i->getDescription() + "', '"
+ i->getEffect() + "', '"
+ i->getTarget() + "', "
+ to_string(i->getValue()) + ", '"
+ i->getStatusEffect() + "', "
+ to_string(i->isReusable()) + ");";

cout << "DEBUG: SQL = " << sql << endl;
executeSQL(sql);
cout << "DEBUG: Item saved successfully" << endl;
}

// save defeated enemies
cout << "DEBUG: Saving defeated enemies" << endl;
for (auto& pair : stats.itemUsageCount)
{
cout << "DEBUG: item usage - " << pair.first 
<< " used " << pair.second << " times" << endl;
}

// save stats
cout << "DEBUG: Saving stats" << endl;
cout << "DEBUG: totalMonstersDefeated = " 
<< stats.totalMonstersDefeated << endl;

saveStats(characterId, stats);

cout << "Game saved!" << endl;
}

void Database::saveStats(int characterId, const Stats& stats)
{
    // save total monsters defeated
    executeSQL("INSERT INTO statistics (character_id, totalMonstersDefeated) VALUES ("
               + to_string(characterId) + ", "
               + to_string(stats.totalMonstersDefeated) + ");");

    // save item usage
    for (auto& pair : stats.itemUsageCount)
    {
        string sql = "INSERT INTO item_usage (character_id, itemName, "
                     "timesUsed, monstersDefeated) VALUES ("
                     + to_string(characterId) + ", '"
                     + pair.first + "', "
                     + to_string(pair.second) + ", "
                     + to_string(stats.itemKillCount.count(pair.first) ?
                       stats.itemKillCount.at(pair.first) : 0) + ");";
        executeSQL(sql);
    }

    // save monster usage
    for (auto& pair : stats.monsterUsageCount)
    {
        string sql = "INSERT INTO monster_usage (character_id, monsterName, "
                     "timesUsed) VALUES ("
                     + to_string(characterId) + ", '"
                     + pair.first + "', "
                     + to_string(pair.second) + ");";
        executeSQL(sql);
    }
}

void Database::saveDefeatedEnemy(int characterId, const string& areaName,
    const string& enemyName)
{
cout << "DEBUG saveDefeatedEnemy: characterId=" << characterId 
<< " area=" << areaName 
<< " enemy=" << enemyName << endl;

string sql = "INSERT INTO defeated_enemies (character_id, areaName, enemyName) "
"VALUES (" + to_string(characterId) + ", '"
+ areaName + "', '"
+ enemyName + "');";

cout << "DEBUG SQL: " << sql << endl;

char* errMsg = nullptr;
int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
if (result != SQLITE_OK)
{
cout << "DEBUG SQL ERROR: " << errMsg << endl;
sqlite3_free(errMsg);
}
else
{
cout << "DEBUG: Insert succeeded, rows affected = " 
<< sqlite3_changes(db) << endl;
}
}

Stats Database::loadStats(int characterId)
{
    Stats stats;

    // load total monsters defeated
    sqlite3_stmt* stmt;
    string sql = "SELECT totalMonstersDefeated FROM statistics WHERE character_id = "
                 + to_string(characterId) + ";";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
            stats.totalMonstersDefeated = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
    }

    // load item usage
    sql = "SELECT itemName, timesUsed, monstersDefeated FROM item_usage "
          "WHERE character_id = " + to_string(characterId) + ";";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            stats.itemUsageCount[name] = sqlite3_column_int(stmt, 1);
            stats.itemKillCount[name] = sqlite3_column_int(stmt, 2);
        }
        sqlite3_finalize(stmt);
    }

    // load monster usage
    sql = "SELECT monsterName, timesUsed FROM monster_usage "
          "WHERE character_id = " + to_string(characterId) + ";";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            stats.monsterUsageCount[name] = sqlite3_column_int(stmt, 1);
        }
        sqlite3_finalize(stmt);
    }

    return stats;
}

Character* Database::loadCharacter(int id, Stats& stats,
                                    vector<Area*>& world)
{
    if (!db) return nullptr;

    // load character name
    string sql = "SELECT name FROM characters WHERE id = " 
                 + to_string(id) + ";";
    
    sqlite3_stmt* stmt;
    string charName;
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
            charName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        sqlite3_finalize(stmt);
    }

    if (charName.empty()) return nullptr;

    Character* player = new Character(charName);

    // load monsters
    sql = "SELECT name, hp, maxHp, strength, specialEffect, effectChance, "
          "effectDuration, critChance, critMultiplier FROM monsters "
          "WHERE character_id = " + to_string(id) + ";";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            string name = reinterpret_cast<const char*>(
                          sqlite3_column_text(stmt, 0));
            int hp        = sqlite3_column_int(stmt, 1);
            int maxHp     = sqlite3_column_int(stmt, 2);
            int strength  = sqlite3_column_int(stmt, 3);
            string effect = reinterpret_cast<const char*>(
                            sqlite3_column_text(stmt, 4));
            int effectChance    = sqlite3_column_int(stmt, 5);
            int effectDuration  = sqlite3_column_int(stmt, 6);
            int critChance      = sqlite3_column_int(stmt, 7);
            float critMult      = sqlite3_column_double(stmt, 8);

            Monster* m = new Monster(name, maxHp, strength, effect,
                                     effectChance, effectDuration,
                                     critChance, critMult);
            // restore current hp
            int damage = maxHp - hp;
            if (damage > 0) m->takeDamage(damage);

            player->addMonster(m);
        }
        sqlite3_finalize(stmt);
    }

    // load items
    sql = "SELECT name, description, effect, target, value, statusEffect, "
          "reusable FROM items WHERE character_id = " + to_string(id) + ";";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            string name   = reinterpret_cast<const char*>(
                            sqlite3_column_text(stmt, 0));
            string desc   = reinterpret_cast<const char*>(
                            sqlite3_column_text(stmt, 1));
            string effect = reinterpret_cast<const char*>(
                            sqlite3_column_text(stmt, 2));
            string target = reinterpret_cast<const char*>(
                            sqlite3_column_text(stmt, 3));
            int value     = sqlite3_column_int(stmt, 4);
            string statusEffect = reinterpret_cast<const char*>(
                                  sqlite3_column_text(stmt, 5));
            bool reusable = sqlite3_column_int(stmt, 6);

            player->addItem(new Item(name, desc, effect, target,
                                     value, statusEffect, reusable));
        }
        sqlite3_finalize(stmt);
    }

    // load defeated enemies and remove them from world
    sql = "SELECT areaName, enemyName FROM defeated_enemies "
          "WHERE character_id = " + to_string(id) + ";";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            string areaName  = reinterpret_cast<const char*>(
                               sqlite3_column_text(stmt, 0));
            string enemyName = reinterpret_cast<const char*>(
                               sqlite3_column_text(stmt, 1));

            // find the area and remove the enemy
            for (Area* area : world)
            {
                if (area->getName() == areaName)
                {
                    for (Enemy* e : area->getEnemies())
                    {
                        if (e->getName() == enemyName)
                        {
                            area->removeEnemy(e);
                            delete e;
                            break;
                        }
                    }
                }
            }
        }
        sqlite3_finalize(stmt);
    }

    // load stats
    stats = loadStats(id);

    cout << "Hero " << charName << " loaded!" << endl;
    return player;
}

vector<HeroSummary> Database::getAllHeroes()
{
    vector<HeroSummary> heroes;
    if (!db) return heroes;

    string sql = R"(
        SELECT c.id, c.name,
               COUNT(DISTINCT m.id) as monsterCount,
               COALESCE(s.totalMonstersDefeated, 0) as totalDefeated,
               COUNT(DISTINCT i.id) as itemCount
        FROM characters c
        LEFT JOIN monsters m ON m.character_id = c.id
        LEFT JOIN statistics s ON s.character_id = c.id
        LEFT JOIN items i ON i.character_id = c.id
        GROUP BY c.id;
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            HeroSummary h;
            h.id                   = sqlite3_column_int(stmt, 0);
            h.name                 = reinterpret_cast<const char*>(
                                     sqlite3_column_text(stmt, 1));
            h.monsterCount         = sqlite3_column_int(stmt, 2);
            h.totalMonstersDefeated = sqlite3_column_int(stmt, 3);
            h.itemCount             = sqlite3_column_int(stmt, 4); 
            heroes.push_back(h);
        }
        sqlite3_finalize(stmt);
    }

    return heroes;
}

void Database::deleteHero(int id)
{
    executeSQL("DELETE FROM monsters WHERE character_id = " 
               + to_string(id) + ";");
    executeSQL("DELETE FROM items WHERE character_id = " 
               + to_string(id) + ";");
    executeSQL("DELETE FROM statistics WHERE character_id = " 
               + to_string(id) + ";");
    executeSQL("DELETE FROM item_usage WHERE character_id = " 
               + to_string(id) + ";");
    executeSQL("DELETE FROM monster_usage WHERE character_id = " 
               + to_string(id) + ";");
    executeSQL("DELETE FROM defeated_enemies WHERE character_id = " 
               + to_string(id) + ";");
    executeSQL("DELETE FROM characters WHERE id = " 
               + to_string(id) + ";");
    cout << "Hero deleted." << endl;
}

void Database::printStats(int characterId)
{
    cout << "\n=== STATISTICS ===" << endl;

    sqlite3_stmt* stmt;

    // total monsters defeated
    string sql = "SELECT totalMonstersDefeated FROM statistics "
                 "WHERE character_id = " + to_string(characterId) + ";";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
            cout << "Total monsters defeated: " 
                 << sqlite3_column_int(stmt, 0) << endl;
        sqlite3_finalize(stmt);
    }

    // most used item
    sql = "SELECT itemName, timesUsed FROM item_usage "
          "WHERE character_id = " + to_string(characterId)
          + " ORDER BY timesUsed DESC LIMIT 1;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
            cout << "Most used item: "
                 << sqlite3_column_text(stmt, 0) << " ("
                 << sqlite3_column_int(stmt, 1) << " times)" << endl;
        sqlite3_finalize(stmt);
    }

    // item with most kills
    sql = "SELECT itemName, monstersDefeated FROM item_usage "
          "WHERE character_id = " + to_string(characterId)
          + " ORDER BY monstersDefeated DESC LIMIT 1;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
            cout << "Most kills by item: "
                 << sqlite3_column_text(stmt, 0) << " ("
                 << sqlite3_column_int(stmt, 1) << " kills)" << endl;
        sqlite3_finalize(stmt);
    }

    // most used monster
    sql = "SELECT monsterName, timesUsed FROM monster_usage "
          "WHERE character_id = " + to_string(characterId)
          + " ORDER BY timesUsed DESC LIMIT 1;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
            cout << "Most used monster: "
                 << sqlite3_column_text(stmt, 0) << " ("
                 << sqlite3_column_int(stmt, 1) << " times)" << endl;
        sqlite3_finalize(stmt);
    }
}