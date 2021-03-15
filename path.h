#include <string>

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: path.h
*/

using namespace std;

#ifndef PATH_H
#define PATH_H
class Room;

class Path {
public:
	Path(string n = "Generic Path", int d = 0, Room* r = nullptr) {
		direction = d;
		attachedRoom = r;
		name = n;
	}
	Room* travel() {
		return attachedRoom;
	}
	int getDir() {
		return direction;
	}
	string getName() {
		return name;
	}
	void setRoom(Room* r) {
		attachedRoom = r;
	}
private:
	int direction;
	Room* attachedRoom;
	string name;
};


#endif
