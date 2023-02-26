/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: npc.h
*/

#ifndef NPC_H
#define NPC_H

#include <vector>
#include <string>

#include "player.h"

enum class Aggression 
{
	PASSIVE,
	NEUTERAL,
	AGGRESSIVE,
};

class Object;


class NPC : public Player {
public:

	NPC(std::string aName = "miner", double aHp = 30, double aStrength = 1.2, int aAgility = 10, double aSizeCapacity = 30, double aWeightCapacity = 70, Aggression aAggression = Aggression::NEUTERAL);

	std::vector<Object*> takeHit(Player* enemy, double damage) override;

	std::vector<Object*> update(std::vector<Player*> players) override;

private:

	Aggression mAggression;

};


#endif
