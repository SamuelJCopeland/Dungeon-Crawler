/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: gameClock.h
*/

#ifndef GAMECLOCK_H
#define GAMECLOCK_H

#include <vector>

class Room;
class Player;

class Clock {
public:

	Clock();

	void clockUpdate(Room* room, std::vector<Player*> players);

	void updateLights(Room* room, std::vector<Player*> players);

private:

	unsigned int mTime{ 0 };
};
#endif
