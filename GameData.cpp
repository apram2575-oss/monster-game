#include "GameData.h"
#include "Area.h"
#include "Monster.h"
#include "Enemy.h"

vector<Area*> setupWorld() 
{

    Area* forest = new Area("Cursed Forest", "A dark and mysterious forest filled with danger.");

    Enemy* hermit = new Enemy("Hermit", "Cursed Forest");
    hermit->addMonster(new Monster("Rat", 5, 2));
    hermit->addMonster(new Monster("Rat", 5, 2));
    hermit->addMonster(new Monster("Bat", 10, 5));
    forest->addEnemy(hermit);

    Enemy* bandit = new Enemy("Bandit Leader", "Cursed Forest");
    bandit->addMonster(new Monster("Goon With Knife", 10, 10));
    bandit->addMonster(new Monster("Buff Goon", 20, 4));
    bandit->addMonster(new Monster("Pitbull", 12, 3));
    forest->addEnemy(bandit);
    
    Enemy* spirit = new Enemy("Forest Spirit", "Cursed Forest");
    spirit->addMonster(new Monster("Ghost", 20, 5));
    spirit->addMonster(new Monster("Cursed Tree", 10, 2));
    spirit->addMonster(new Monster("Cursed Tree", 10, 2));
    spirit->addMonster(new Monster("Vengeful Spirit", 30, 8));
    forest->addEnemy(spirit);


    Area* cave = new Area("Dark Cave", "An ominous cave that echoes with the sounds of unseen creatures.");
   
    Enemy* cavedweller = new Enemy("Cave Dweller", "Dark Cave");
    cavedweller->addMonster(new Monster("Rat", 5, 2));
    cavedweller->addMonster(new Monster("Spider", 10, 5));
    cavedweller->addMonster(new Monster("Spider", 10, 5));
    cavedweller->addMonster(new Monster("Giant Spider", 20, 7));
    cave->addEnemy(cavedweller);

    Enemy* facelessmonster = new Enemy("Faceless Monster", "Dark Cave");
    facelessmonster->addMonster(new Monster("Faceless Minion", 15, 3));
    facelessmonster->addMonster(new Monster("Faceless Brute", 30, 10));
    facelessmonster->addMonster(new Monster("Faceless Monster", 40, 15));
    cave->addEnemy(facelessmonster);

    Enemy* goblin = new Enemy("Goblin", "Dark Cave");
    goblin->addMonster(new Monster("Hydra", 70, 20));
    cave->addEnemy(goblin);

    Area* castle = new Area("Castle", "A grand castle seemingly safe, but something is off about this place.");

    Enemy* raiders = new Enemy("Raider Group", "Castle");
    raiders->addMonster(new Monster("Amelia, The Leader", 15, 12));
    raiders->addMonster(new Monster("Damon, The Skillshot", 12, 15));
    raiders->addMonster(new Monster("Levok, The Butcher", 25, 10));
    raiders->addMonster(new Monster("Radon, The Archer", 10, 30));
    castle->addEnemy(raiders);
    
    Enemy* knight = new Enemy("Knight", "Castle");
    knight->addMonster(new Monster("Knight", 30, 15));
    knight->addMonster(new Monster("Elite Knight", 50, 20));
    castle->addEnemy(knight);

    Enemy* king = new Enemy("Evil King", "Castle");
    king->addMonster(new Monster("Cursed Knight", 40, 20));
    king->addMonster(new Monster("Cursed Knight", 40, 20));
    king->addMonster(new Monster("Dragon", 100, 30));
    castle->addEnemy(king);

    return {forest, cave, castle};


}