#include "room.h"
#include "npc.h"
#include "weapon.h"
#include "container.h"
#include "food.h"
#include "light.h"

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: gameClock.h
*/

#ifndef GAMECLOCK_H
#define GAMECLOCK_H
class Clock {
public:
	Clock() {
		time = 0;
	}

	void clockUpdate(Room* room, vector<Player*> players) {
		time++;
		vector<Object*> objects;
		if (room != nullptr) {
			objects = recursiveFindObjects(room->getObjects());
		}
		for (Player* p : players) {

			if (p->getHP() < p->getMaxHealth()) {
				if (p->getHP() + .5 <= p->getMaxHealth()) {
					p->setHP(p->getHP() + .5);
				}
				else {
					p->setHP(p->getMaxHealth());
				}
			}
			for (Object* o : recursiveFindObjects(p->getInventory())) {
				objects.push_back(o);
			}
		}
		for (Player* p : players) {
			for (Object* o : p->update(players)) {
				room->addObject(o);
			}
		}
		bool light = false;
		for (Object* o : objects) {
			if (o->updateLight()) {
				light = true;
			}
			if (o->time_til_dead() == 0 && splitString(o->getName(), ' ')[0] == "match") {
				bool foundContainer = false;
				Object* container;
				//Check the room
				if (findObject("match", room->getObjects()) == o) {
					room->removeObject(o);
					delete o;
					foundContainer = true;
				}
				else {
					for (Object* ob : room->getObjects()) {
						container = recursiveFindContainer(o, ob);
						if (container != nullptr) {
							container->takeOut(o);
							delete o;
							foundContainer = true;
							break;
						}
					}
				}

				//check the players
				if (!foundContainer) {
					for (Player* play : players) {
						if (findObject("match", play->getInventory()) == o) {
							play->removeObject(o);
							delete o;
							foundContainer = true;
						}
						else {
							for (Object* ob : play->getInventory()) {
								container = recursiveFindContainer(o, ob);
								if (container != nullptr) {
									container->takeOut(o);
									delete o;
									foundContainer = true;
									break;
								}
							}
						}
					}
				}
			}
		}
		if (room != nullptr) {
			room->setDark(!light);
		}
	}
	void updateLights(Room* room, vector<Player*> players) {
		vector<Object*> objects;
		if (room != nullptr) {
			objects = recursiveFindObjects(room->getObjects());
		}
		for (Player* p : players) {
			
			for (Object* o : recursiveFindObjects(p->getInventory())) {
				objects.push_back(o);
			}
		}
		
		bool light = false;
		for (Object* o : objects) {
			if (o->updateLight()) {
				light = true;
			}
			if (o->time_til_dead() == 0 && splitString(o->getName(), ' ')[0] == "match") {
				bool foundContainer = false;
				Object* container;
				//Check the room
				if (findObject("match", room->getObjects()) == o) {
					room->removeObject(o);
					delete o;
					foundContainer = true;
				}
				else {
					for (Object* ob : room->getObjects()) {
						container = recursiveFindContainer(o, ob);
						if (container != nullptr) {
							container->takeOut(o);
							delete o;
							foundContainer = true;
							break;
						}
					}
				}

				//check the players
				if (!foundContainer) {
					for (Player* play : players) {
						if (findObject("match", play->getInventory()) == o) {
							play->removeObject(o);
							delete o;
							foundContainer = true;
						}
						else {
							for (Object* ob : play->getInventory()) {
								container = recursiveFindContainer(o, ob);
								if (container != nullptr) {
									container->takeOut(o);
									delete o;
									foundContainer = true;
									break;
								}
							}
						}
					}
				}
			}
		}
		if (room != nullptr) {
			room->setDark(!light);
		}
	}
private:
	unsigned int time;
};
#endif
