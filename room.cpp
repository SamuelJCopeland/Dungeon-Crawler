#include"room.h"
#include <map>
#include <algorithm>

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: room.cpp
*/

using namespace std;

string Room::getDescription(){
	map<int, string> dirMap;
	dirMap.insert(pair<int, string>(0, "north"));
	dirMap.insert(pair<int, string>(1, "northeast"));
	dirMap.insert(pair<int, string>(2, "east"));
	dirMap.insert(pair<int, string>(3, "southeast"));
	dirMap.insert(pair<int, string>(4, "south"));
	dirMap.insert(pair<int, string>(5, "southwest"));
	dirMap.insert(pair<int, string>(6, "west"));
	dirMap.insert(pair<int, string>(7, "northwest"));
	dirMap.insert(pair<int, string>(8, "up"));
	dirMap.insert(pair<int, string>(9, "down"));
	string fullDes;
	if (!isDark) {
		fullDes = name + "\n" + description;

		for (Path* p : paths) {
			if (p->getName()[0] == 'a' || p->getName()[0] == 'e' || p->getName()[0] == 'i' || p->getName()[0] == 'o' || p->getName()[0] == 'u') {
				fullDes += "\nThere is an " + p->getName() + " to your " + dirMap[p->getDir()];
			}
			else {
				fullDes += "\nThere is a " + p->getName() + " to your " + dirMap[p->getDir()];
			}
			
		}

		fullDes += "\nThere are the following items here:\n";

		for (Object* o : objects) {
			fullDes += "\t" + o->getName();
			if (o->is_open() && o->getInventory().size() > 0) {
				fullDes += o->printInventory("\t") + "\n";
			}
			else {
				fullDes += "\n";
			}
		}

		for (Player* p : players) {
			if (p->getName() != "you") {
				fullDes += "There is a " + p->getName() + " here\n";
				if (p->getInventory().size() > 0) {
					fullDes += "The " + p->getName() + " is holding:\n" + p->printInventory() + "\n";
				}
			}
		}
	}
	else {
		fullDes = name + "\nIt is too dark to see anything.\n";
	}
	return fullDes;
}

void Room::addObject(Object* o){
	objects.push_back(o);
}

void Room::removeObject(Object* o){
	vector<Object*>::iterator position = find(objects.begin(), objects.end(), o);
	if (position != objects.end()) {
		objects.erase(position);
	}
}

void Room::addPlayer(Player* p){
	players.push_back(p);
}

void Room::removePlayer(Player* p) {
	vector<Player*>::iterator position = find(players.begin(), players.end(), p);
	if (position != players.end()) {
		players.erase(position);
	}
}

void Room::addPath(Path* p){
	paths.push_back(p);
}

void Room::removePath(Path* p){
	vector<Path*>::iterator position = find(paths.begin(), paths.end(), p);
	if (position != paths.end()) {
		paths.erase(position);
	}
}
