/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: player.h
*/

#ifndef PLAYER_H
#define PLAYER_H

#include<vector>
#include <string>

#define MAX_AGILITY 50

class Object;
class Weapon;

class Player
{
public:

	Player(std::string aName = "you", double aHP = 25, double aStrength = 1, int aAgility = 5, double aSizeCapacity = 20, double aWeightCapacity = 50);

	std::string name();

	double agility();

	double hp();

	double maxHP();

	std::vector<Object*> inventory();

	std::string printInventory();

	void hp(double health);

	//This player attacks another player
	std::vector<Object*> attack(Player* enemy, Object* weapon);

	//The enemy attacks the player return inventory if the player dies
	virtual std::vector<Object*> takeHit(Player* enemy, double damage);

	bool addObject(Object* o);
	
	bool removeObject(Object* o);

	bool isAlive();

	virtual std::vector<Object*> update(std::vector<Player*> players);

	static Player* findPlayer(std::string name, std::vector<Player*> pl);

protected:

	std::string mName;

	//How much size the player can hold
	double mSizeCapacity;

	//How much weight the player can hold
	double mWeightCapacity;

	//How much health the player has
	double mHitpoints;

	//How strong the player is
	double mStrength;

	//How easily the player can dodge attacks
	int mAgility;

	//What the player has in their inventory
	std::vector<Object*> mInventory;

	bool mAlive = true;

	double mMaxHealth;

};

#endif
