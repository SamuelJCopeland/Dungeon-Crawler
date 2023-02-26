/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: path.h
*/

#ifndef PATH_H
#define PATH_H

#include <string>

class Room;

enum class Compass
{
	NORTH,
	NORTHEAST,
	EAST,
	SOUTHEAST,
	SOUTH,
	SOUTHWEST,
	WEST,
	NORTHWEST,
	UP,
	DOWN,

	INVALIDDIRECTION,
};

class Path 
{
public:

	Path(std::string aName = "Generic Path", Compass aDirection = Compass::NORTH, Room* aRoom = nullptr);

	Compass dir();

	std::string name();

	void room(Room* aRoom);

	static std::string compassToString(Compass aCompass);

	static Compass stringToCompass(std::string aInput);

	Room* travel();

private:

	Compass mDirection;
	Room* mAttachedRoom;
	std::string mName;

};


#endif
