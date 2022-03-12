#ifndef PLAYER_H
#define PLAYER_H

#include<vector>
#include <string>
#include <time.h>
#include <iostream>
#include <algorithm>
#include "weapon.h"

#define MAX_AGILITY 50

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: player.h
*/

using namespace std;

class Player {
public:
	Player(string n = "you", double hp = 25, double s = 1, int a = 5, double sC = 20, double wC = 50) {
		name = n;
		sizeCapacity = sC;
		weightCapacity = wC;
		hitpoints = hp;
		strength = s;
		agility = a;
		maxHealth = hp;
	}
	//This player attacks another player
	vector<Object*> attack(Player* enemy, Object* weapon);
	//The enemy attacks the player return inventory if the player dies
	virtual vector<Object*> takeHit(Player* enemy, double damage);
	
	bool removeObject(Object* o);
	bool addObject(Object* o);
	bool isAlive() {
		return alive;
	}

	string printInventory();
	string getName();
	double getHP();
	void setHP(double health) {
		hitpoints = health;
	}
	double getAgility();
	double getMaxHealth() {
		return maxHealth;
	}

	vector<Object*> getInventory() {
		return inventory;
	}

	virtual vector<Object*> update(vector<Player*> players) { return vector<Object*>{}; }

protected:
	string name;
	//How much size the player can hold
	double sizeCapacity;
	//How much weight the player can hold
	double weightCapacity;
	//How much health the player has
	double hitpoints;
	//How strong the player is
	double strength;
	//How easily the player can dodge attacks
	int agility;
	//What the player has in their inventory
	vector<Object*> inventory;
	bool alive = true;
	double maxHealth;
};

class PC : public Player {
public:
	vector<Object*> takeHit(Player* enemy, double damage) {
		int agility = this->agility;
		srand(time(NULL));

		int damageChance = (int)rand() % 4 + 1 + enemy->getAgility() - ((int)rand() % 4 + 1 + agility);

		if (damageChance > 0) {
			hitpoints -= damage;
			if (hitpoints <= 0) {
				cout << "You died.\n";
				alive = false;
				return inventory;
			}
			else {
				cout << "The " + enemy->getName() + " hit " << name << ".\n";
			}
		}
		else {
			cout << "You dodged the attack.\n";
		}
		return vector<Object*>{};
	}
};

Player* findPlayer(string name, vector<Player*> pl);

#endif
