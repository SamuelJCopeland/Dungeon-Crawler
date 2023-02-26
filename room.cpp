#include"room.h"

#include <map>
#include <algorithm>
#include <string>
#include <vector>


#include "path.h"
#include "object.h"
#include "player.h"

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: room.cpp
*/

Room::Room(std::string aName, std::string aDescription, std::vector<Path*> aPaths, std::vector<Player*> aPlayers, std::vector<Object*> aObjects, bool aCanDark, bool aIsDark)
	: mName(aName)
	, mDescription(aDescription)
	, mIsDark(aIsDark)
	, mCanDark(aCanDark)
	, mPaths(aPaths)
	, mObjects(aObjects)
	, mPlayers(aPlayers)
{}

std::string Room::name()
{
	return mName;
}

std::string Room::description()
{

	std::string fullDes;

	if (!mIsDark) 
	{
		fullDes = mName + "\n" + mDescription;

		for (Path* p : mPaths) 
		{

			if (p->name()[0] == 'a' || p->name()[0] == 'e' || p->name()[0] == 'i' || p->name()[0] == 'o' || p->name()[0] == 'u') 
			{
				fullDes += "\nThere is an " + p->name() + " to your " + Path::compassToString(p->dir());
			}
			else 
			{
				fullDes += "\nThere is a " + p->name() + " to your " + Path::compassToString(p->dir());
			}
			
		}

		fullDes += "\nThere are the following items here:\n";

		for (Object* o : mObjects) 
		{
			fullDes += "\t" + o->name();

			if (o->isOpen() && o->inventory().size() > 0) 
			{
				fullDes += o->printInventory("\t") + "\n";
			}
			else
			{
				fullDes += "\n";
			}

		}

		for (Player* p : mPlayers)
		{

			if (p->name() != "you") 
			{
				fullDes += "There is a " + p->name() + " here\n";

				if (p->inventory().size() > 0) 
				{
					fullDes += "The " + p->name() + " is holding:\n" + p->printInventory() + "\n";
				}

			}

		}

	}
	else
	{
		fullDes = mName + "\nIt is too dark to see anything.\n";
	}

	return fullDes;
}

bool Room::isDark()
{
	return mIsDark;
}

std::vector<Path*> Room::paths()
{
	return mPaths;
}

std::vector<Player*> Room::players()
{
	return mPlayers;
}

std::vector<Object*> Room::objects()
{
	return mObjects;
}

void Room::isDark(bool dark)
{

	if (mCanDark)
	{
		mIsDark = dark;
	}

}

void Room::addObject(Object* o)
{
	mObjects.push_back(o);
}

void Room::removeObject(Object* o)
{
	std::vector<Object*>::iterator position = std::find(mObjects.begin(), mObjects.end(), o);

	if (position != mObjects.end()) 
	{
		mObjects.erase(position);
	}

}

void Room::addPlayer(Player* p)
{
	mPlayers.push_back(p);
}

void Room::removePlayer(Player* p) 
{
	std::vector<Player*>::iterator position = std::find(mPlayers.begin(), mPlayers.end(), p);

	if (position != mPlayers.end()) 
	{
		mPlayers.erase(position);
	}

}

void Room::addPath(Path* p)
{
	mPaths.push_back(p);
}

void Room::removePath(Path* p)
{
	std::vector<Path*>::iterator position = std::find(mPaths.begin(), mPaths.end(), p);

	if (position != mPaths.end()) 
	{
		mPaths.erase(position);
	}

}
