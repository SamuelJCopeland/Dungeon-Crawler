/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: room.h
*/

#ifndef ROOM_H
#define ROOM_H

#include<string>
#include <vector>

class Object;
class Player;
class Path;

class Room 
{
public:

	Room(std::string aName = "Blank Room", std::string aDescription = "You are in a formless void. There is nothing but darkness in all directions", 
		std::vector<Path*> aPaths = {}, std::vector<Player*> aPlayers = {}, std::vector<Object*> aObjects = {}, bool aCanDark = false, bool aIsDark = false);

	std::string name();

	std::string description();

	bool isDark();

	std::vector<Path*> paths();

	std::vector<Player*> players();

	std::vector<Object*> objects();

	void isDark(bool dark);

	void addObject(Object* o);

	void removeObject(Object* o);

	void addPlayer(Player* p);

	void removePlayer(Player* p);

	void addPath(Path* p);

	void removePath(Path* p);

private:

	//The name of the room
	std::string mName;

	//A description of the room
	std::string mDescription;

	//vector of any paths out of the room
	std::vector<Path*> mPaths;

	//vector of all items held in the room
	std::vector<Object*> mObjects;

	//vector of all players and npcs that are in the room
	std::vector<Player*> mPlayers;

	bool mIsDark;

	bool mCanDark;
};

#endif
