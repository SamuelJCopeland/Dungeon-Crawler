#ifndef ROOM_H
#define ROOM_H

#include<string>
#include <vector>
#include "path.h"
#include "player.h"

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: room.h
*/

using namespace std;

class Room {
public:
	Room(string n = "Blank Room", string d = "You are in a formless void. There is nothing but darkness in all directions", vector<Path*> p = {}, vector<Player*> plyrs = {}, vector<Object*> o = {}, bool cDark = false, bool dark = false) {
		name = n;
		description = d;
		paths = p;
		objects = o;
		players = plyrs;
		canDark = cDark;
		isDark = dark;
	}

	string getDescription();
	string getName() {
		return name;
	}
	vector<Path*> getPaths() {
		return paths;
	}
	vector<Player*> getPlayers() {
		return players;
	}
	vector<Object*> getObjects() {
		return objects;
	}
	void addObject(Object* o);
	void removeObject(Object* o);
	void addPlayer(Player* p);
	void removePlayer(Player* p);
	void addPath(Path* p);
	void removePath(Path* p);
	void setDark(bool dark) {
		if (canDark) {
			isDark = dark;
		}
	}
	bool is_dark() {
		return isDark;
	}
private:
	//The name of the room
	string name;
	//A description of the room
	string description;
	//vector of any paths out of the room
	vector<Path*> paths;
	//vector of all items held in the room
	vector<Object*> objects;
	//vector of all players and npcs that are in the room
	vector<Player*> players;
	bool isDark;
	bool canDark;
};

#endif
