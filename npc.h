#include "player.h"

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: npc.h
*/

using namespace std;

#ifndef NPC_H
#define NPC_H

#define PASSIVE 0
#define NEUTERAL 1
#define AGGRESSIVE 2


class NPC : public Player {
public:
	NPC(string n = "miner", double hp = 30, double s = 1.2, int a = 10, double sC = 30, double wC = 70, int aggress = NEUTERAL) {
		name = n;
		sizeCapacity = sC;
		weightCapacity = wC;
		hitpoints = hp;
		strength = s;
		agility = a;
		aggression = aggress;
	}

	vector<Object*> takeHit(Player* enemy, double damage) override;
	vector<Object*> update(vector<Player*> players) override;
private:
	int aggression;
};


#endif
