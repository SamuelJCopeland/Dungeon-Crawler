#include "npc.h"

#include <time.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include "weapon.h"

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
* 
* File: npc.cpp
*/

NPC::NPC(std::string aName, double aHp, double aStrength, int aAgility, double aSizeCapacity, double aWeightCapacity, Aggression aAggression) 
	: Player(aName, aHp, aStrength, aSizeCapacity, aWeightCapacity)
	, mAggression(aAggression)
{}

std::vector<Object*> NPC::takeHit(Player* enemy, double damage) {
	int dodgeChance = this->mAgility;

	int damageChance = rand() % MAX_AGILITY + 1;

	if (damageChance > dodgeChance) 
	{
		mHitpoints -= damage;

		if (mHitpoints <= 0) 
		{
			std::cout << "The " << mName << " " << "died.\n";
			mAlive = false;
			return mInventory;
		}
		else 
		{
			std::cout << enemy->name() << " hit the " << mName << ".\n";
		}

	}
	else
	{
		std::cout << "The " << mName << " dodged the attack.\n";
	}

	if (mAggression == Aggression::NEUTERAL) 
	{
		mAggression = Aggression::AGGRESSIVE;		
	}

	return std::vector<Object*>{};
}

std::vector<Object*> NPC::update(std::vector<Player*> players) 
{

	if (mAggression == Aggression::AGGRESSIVE)
	{
		Player* player = findPlayer("you", players);

		if (player != nullptr) 
		{
			Weapon* fist = new Weapon("fist", 0, 0, "", .5);
			Object* maxWeapon = fist;

			for (Object* o : mInventory)
			{

				if (o->damage() > maxWeapon->damage())
				{
					maxWeapon = o;
				}

			}

			std::vector<Object*> loot = attack(player, maxWeapon);
			delete fist;

			return loot;
		}

	}

	return std::vector<Object*>{};
}