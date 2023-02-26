#include "player.h"

#include <time.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "object.h"
#include "weapon.h"

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: player.cpp
*/

Player::Player(std::string aName, double aHP, double aStrength, int aAgility, double aSizeCapacity, double aWeightCapacity)
	: mName(aName)
	, mSizeCapacity(aSizeCapacity)
	, mWeightCapacity(aWeightCapacity)
	, mHitpoints(aHP)
	, mStrength(aStrength)
	, mAgility(aAgility)
	, mMaxHealth(aHP)
{}

std::string Player::name()
{
	return mName;
}

double Player::agility()
{
	return mAgility;
}

double Player::hp()
{
	return mHitpoints;
}

double Player::maxHP()
{
	return mMaxHealth;
}

std::vector<Object*> Player::inventory()
{
	return mInventory;
}

std::string Player::printInventory()
{
	std::string iDescription = "";

	for (Object* o : mInventory) 
	{
		iDescription += "\t" + o->name();

		if (o->isOpen() && o->inventory().size() > 0)
		{
			iDescription += o->printInventory("\t") + "\n";
		}
		else
		{
			iDescription += "\n";
		}

	}

	return iDescription;
}

void Player::hp(double health)
{
	mHitpoints = health;
}

std::vector<Object*> Player::attack(Player* enemy, Object* weapon)
{
	double damage = weapon->damage();

	if (damage > 0)
	{
		//Total damage = base weapon damage * strength + d4
		damage = damage * mStrength + rand() % 4 + 1;
		return enemy->takeHit(this, damage);
	}
	else
	{
		std::cout << "You swing the " + weapon->name() + " at the " + enemy->name() + ", it has no effect.\n";
		return std::vector<Object*>();
	}

}

std::vector<Object*> Player::takeHit(Player* enemy, double damage) 
{
	int dodgeChance = this->mAgility;

	if (dodgeChance < 0)
	{
		dodgeChance = 0;
	}
	
	int damageChance = rand() % MAX_AGILITY + 1;

	if (damageChance > dodgeChance)
	{
		mHitpoints -= damage;

		if (mHitpoints <= 0) 
		{
			std::cout << mName << " " << "died.\n";
			mAlive = false;
			std::vector<Object*> temp = mInventory;
			mInventory = std::vector<Object*>();
			return temp;
		}
		else
		{
			std::cout << enemy->name() << " hit " << mName << ".\n";
		}

	}
	else 
	{
		std::cout << mName << " dodged the attack.\n";
	}

	return std::vector<Object*>{};
}

bool Player::addObject(Object* o)
{

	if (mWeightCapacity - o->weight() < 0 || mSizeCapacity - o->size() < 0)
	{
		return false;
	}
	else
	{
		mWeightCapacity -= o->weight();
		mSizeCapacity -= o->size() < 0;
		mInventory.push_back(o);
		return true;
	}

}

bool Player::removeObject(Object* o)
{
	std::vector<Object*>::iterator position = std::find(mInventory.begin(), mInventory.end(), o);

	if (position != mInventory.end())
	{
		mInventory.erase(position);
		mWeightCapacity += o->weight();
		mSizeCapacity += o->size();
		return true;
	}

	return false;
}

bool Player::isAlive()
{
	return mAlive;
}

std::vector<Object*> Player::update(std::vector<Player*> players) 
{
	return std::vector<Object*>{}; 
}

Player* Player::findPlayer(std::string name, std::vector<Player*> pl)
{

	for (Player* p : pl) 
	{
		std::string pName = p->name();
		size_t found = pName.find(" (");

		if (pName == name) 
		{
			return p;
		}

	}

	return nullptr;
}
