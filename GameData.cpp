#include "GameData.h"
#include "Area.h"
#include "Monster.h"
#include "Enemy.h"
using namespace std;
vector<Area*> setupWorld() 
{

    Area* forest = new Area("Cursed Forest", "A dark and mysterious forest filled with danger.");

    Enemy* hermit = new Enemy("Hermit", "Cursed Forest");
    hermit->addMonster(new Monster("Rat", 5, 2, "poison", 20, 2, 10, 1.5));
    hermit->addMonster(new Monster("Rat", 5, 2, "poison", 20, 2, 10, 1.5));
    hermit->addMonster(new Monster("Bat", 10, 5, "poison", 100, 2, 10, 1.5));
    forest->addEnemy(hermit);

    Enemy* bandit = new Enemy("Bandit Leader", "Cursed Forest");
    bandit->addMonster(new Monster("Goon With Knife", 10, 5, "bleeding", 50, 3, 50, 2.0));
    bandit->addMonster(new Monster("Buff Goon", 20, 4, "bleeding", 50, 3, 30, 1.5));
    bandit->addMonster(new Monster("Pitbull", 12, 3, "bleeding", 50, 3, 20, 1.5));
    forest->addEnemy(bandit);
    
    Enemy* spirit = new Enemy("Forest Spirit", "Cursed Forest");
    spirit->addMonster(new Monster("Ghost", 20, 5, "curse", 20, 2, 20, 1.5));
    spirit->addMonster(new Monster("Cursed Tree", 10, 2, "curse", 20, 2, 30, 1.5));
    spirit->addMonster(new Monster("Cursed Tree", 10, 2, "curse", 20, 2, 30, 1.5));
    spirit->addMonster(new Monster("Vengeful Spirit", 30, 7, "curse", 50, 3, 30, 1.5));
    forest->addEnemy(spirit);


    Area* cave = new Area("Dark Cave", "An ominous cave that echoes with the sounds of unseen creatures.");
   
    Enemy* cavedweller = new Enemy("Cave Dweller", "Dark Cave");
    cavedweller->addMonster(new Monster("Rat", 5, 2, "poison", 20, 2, 20, 1.5));
    cavedweller->addMonster(new Monster("Spider", 10, 5, "poison", 20, 2, 30, 1.5));
    cavedweller->addMonster(new Monster("Spider", 10, 5, "poison", 20, 2, 30, 1.5));
    cavedweller->addMonster(new Monster("Giant Spider", 20, 7, "poison", 20, 2, 40, 1.5));
    cave->addEnemy(cavedweller);

    Enemy* facelessmonster = new Enemy("Faceless Monster", "Dark Cave");
    facelessmonster->addMonster(new Monster("Faceless Minion", 15, 3, "curse", 20, 2, 20, 1.5));
    facelessmonster->addMonster(new Monster("Faceless Brute", 30, 10, "curse", 20, 2, 20, 1.5));
    facelessmonster->addMonster(new Monster("Faceless Monster", 40, 15, "curse", 60, 3, 50, 2.5));
    cave->addEnemy(facelessmonster);

    Enemy* goblin = new Enemy("Goblin", "Dark Cave");
    goblin->addMonster(new Monster("Hydra", 70, 20, "burn", 60, 2, 20, 3.0));
    cave->addEnemy(goblin);

    Area* castle = new Area("Castle", "A grand castle seemingly safe, but something is off about this place.");

    Enemy* raiders = new Enemy("Raider Group", "Castle");
    raiders->addMonster(new Monster("Amelia, The Leader", 15, 12, "bleeding", 50, 2, 20, 2.0));
    raiders->addMonster(new Monster("Damon, The Skillshot", 12, 10, "bleeding", 50, 2, 60, 2.0));
    raiders->addMonster(new Monster("Levok, The Butcher", 25, 5, "bleeding", 80, 3, 20, 2.0));
    raiders->addMonster(new Monster("Radon, The Archer", 10, 30, "bleeding", 50, 2, 50, 2.0));
    castle->addEnemy(raiders);
    
    Enemy* knight = new Enemy("Knight", "Castle");
    knight->addMonster(new Monster("Knight", 30, 15, "bleeding", 50, 2, 30, 2.0));
    knight->addMonster(new Monster("Elite Knight", 50, 20, "bleeding", 40, 3, 40, 2.0));
    castle->addEnemy(knight);

    Enemy* king = new Enemy("Evil King", "Castle");
    king->addMonster(new Monster("Cursed Knight", 40, 20, "curse", 50, 3, 30, 1.5));
    king->addMonster(new Monster("Cursed Knight", 40, 20, "curse", 50, 3, 30, 1.5));
    king->addMonster(new Monster("Dragon", 100, 30, "burn", 70, 3, 40, 3.0));
    castle->addEnemy(king);

    return {forest, cave, castle};


}

Cave* generateCave(Character* player, int defeatedEnemyCount)
{
    // calculate average hp and strength from player's monsters
    vector<Monster*> playerMonsters = player->getMonsters();
    int totalHp = 0;
    int totalStrength = 0;

    for (Monster* m : playerMonsters)
    {
        totalHp += m->getMaxHealth();
        totalStrength += m->getStrength();
    }

    int avgHp = totalHp / playerMonsters.size();
    int avgStrength = totalStrength / playerMonsters.size();

    // scale monster count with defeated enemies, so it stays fair but gets more challenging as player progresses
    // starts at 2, goes up every 3 defeated enemies, max 6
    int monsterCount = min(2 + (defeatedEnemyCount / 3), 6);

    // pool of random cave monster names
    vector<string> monsterNames = {
        "Cave Bat", "Giant Spider", "Rock Troll",
        "Cave Rat", "Shadow Wolf", "Stone Golem",
        "Dark Slime", "Bone Crawler", "Cave Witch"
    };

    // pool of possible special effects
    vector<pair<string, int>> effects = {
        {"poison", 30},
        {"curse", 20},
        {"burn", 25},
        {"bleeding", 35},
        {"", 0}  // no effect
    };

    Cave* cave = new Cave("Mysterious Cave", "A dark cave filled with unknown creatures.");

    // generate scaled monsters
    for (int i = 0; i < monsterCount; i++)
    {
        // randomize hp and strength around player average
        int hp = avgHp + (rand() % 20 - 10);        // +-10 of average
        int strength = avgStrength + (rand() % 6 - 3); // +-3 of average

        // make sure values dont go too low
        hp = max(hp, 10);
        strength = max(strength, 2);

        // pick random name and effect
        string name = monsterNames[rand() % monsterNames.size()];
        auto effect = effects[rand() % effects.size()];

        cave->addMonster(new Monster(name, hp, strength, effect.first, effect.second, 3));
    }

    // generate scaled reward items
    // scale item potency with defeated enemy count
    int healAmount = 20 + (defeatedEnemyCount * 2);      // scales up
    int damageAmount = 20 + (defeatedEnemyCount * 2);    // scales up
    int boostAmount = 20 + (defeatedEnemyCount * 3);     // scales up faster

    vector<Item*> possibleRewards = {
        new Item("Heal Potion",
                 "Heals your monster for " + to_string(healAmount) + " HP",
                 "heal", "friendly", healAmount),

        new Item("Damage Orb",
                 "Deals " + to_string(damageAmount) + " damage to enemy",
                 "damage", "enemy", damageAmount),

        new Item("Strength Tonic",
                 "Boosts your monster's strength by " + to_string(boostAmount) + "%",
                 "boost", "friendly", boostAmount),

        new Item("Poison Vial",
                 "Poisons the enemy monster",
                 "status", "enemy", 0, "poison"),

        new Item("Curse Scroll",
                 "Curses the enemy monster",
                 "status", "enemy", 0, "curse"),

        new Item("Burn Torch",
                 "Burns the enemy monster",
                 "status", "enemy", 0, "burn")
    };

    // always give one random item
    int firstReward = rand() % possibleRewards.size();
    cave->addRewardItem(possibleRewards[firstReward]);

    // 20% chance for a second item
    int secondRoll = rand() % 100;
    if (secondRoll < 20)
    {
        int secondReward;
        do {
        secondReward = rand() % possibleRewards.size();
        } 
        while (secondReward == firstReward);  // make sure its different
        cave->addRewardItem(possibleRewards[secondReward]);
    }

    // clean up unchosen reward items
    for (int i = 0; i < (int)possibleRewards.size(); i++)
    {
        bool isReward = false;
        for (Item* reward : cave->getRewardItems())
            if (reward == possibleRewards[i])
                isReward = true;
        if (!isReward)
            delete possibleRewards[i];
    }

    return cave;
}