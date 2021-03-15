#include "npc.h"
#include <time.h>
#include <iostream>
#include <algorithm>

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
* 
* File: npc.cpp
*/

using namespace std;

vector<Object*> NPC::takeHit(Player* enemy, double damage) {
	int dodgeChance = this->agility;
	
	srand(time(NULL));

	int damageChance = rand() % MAX_AGILITY + 1;

	if (damageChance > dodgeChance) {
		hitpoints -= damage;
		if (hitpoints <= 0) {
			cout << "The " << name << " " << "died.\n";
			alive = false;
			return inventory;
		}
		else {
			cout << enemy->getName() << " hit the " << name << ".\n";
		}
	}
	else {
		cout << "The " << name << " dodged the attack.\n";
	}
	if (aggression == NEUTERAL) {
		aggression = AGGRESSIVE;
		/*
		Weapon* fist = new Weapon("fist", 0, 0, "", .5);
		Object* maxWeapon = fist;
		for (Object* o : inventory) {
			if (o->getDamage() > maxWeapon->getDamage()) {
				maxWeapon = o;
			}
		}
		vector<Object*> loot = attack(enemy, maxWeapon);
		delete fist;

		if (loot.size() > 0) {
			return loot;
		}
		*/
	}

	return vector<Object*>{};
}

vector<Object*> NPC::update(vector<Player*> players) {
	if (aggression == AGGRESSIVE) {
		Player* player = findPlayer("you", players);
		if (player != nullptr) {
			Weapon* fist = new Weapon("fist", 0, 0, "", .5);
			Object* maxWeapon = fist;
			for (Object* o : inventory) {
				if (o->getDamage() > maxWeapon->getDamage()) {
					maxWeapon = o;
				}
			}
			vector<Object*> loot = attack(player, maxWeapon);
			delete fist;

			return loot;
		}
	}
	return vector<Object*>{};
}