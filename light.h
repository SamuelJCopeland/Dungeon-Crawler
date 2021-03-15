#include "object.h"
#define MATCH_LIFE 3

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: light.h
*/

using namespace std;

#ifndef LIGHT_H
#define LIGHT_H
class Light : public Object {
public:
	Light(string n = "generic_light", double w = 1, double s = 1, string d = "There is nothing special about the generic_light.", int TTD = 1000, bool needLight = false, bool canL = false) {
		weight = w;
		size = s;
		description = d;
		timeTilDead = TTD;
		needsLighter = needLight;
		nameOut = n + " (out)";
		nameLit = n + " (lit)";
		nameDead = n + " (dead)";
		name = nameOut;
		canLight = canL;
	}

	bool light(Object* lightingObject) override {
		if (!needsLighter) {
			if (timeTilDead > 0) {
				isLit = true;
				name = nameLit;
				return true;
			}
			else {
				name = nameDead;
				return false;
			}
		}
		else {
			if (lightingObject != nullptr && lightingObject->isLitCanLight(this)) {
				if (timeTilDead > 0) {
					name = nameLit;
					isLit = true;
					return true;
				}
				else {
					name = nameDead;
					return false;
				}

			}
			else { 
				return false;
			}
		}
	}
	bool putOut() override{
		if (isLit) {
			isLit = false;
			name = nameOut;
			return true;
		}
		else {
			return false;
		}
	}
	bool updateLight() override {
		if (isLit) {
			timeTilDead--;
			if (timeTilDead == 0) {
				isLit = false;
				name = nameDead;
				cout << "The " + splitString(name, ' ')[0] + " went out.\n";
				return false;
			}
			else {
				return true;
			}
		}
		else {
			return false;
		}
	}
	bool isLitCanLight(Object* o) override {
		return isLit && canLight;
	}
	int time_til_dead() {
		return timeTilDead;
	}
private:
	int timeTilDead;
	bool needsLighter;
	bool isLit = false;
	bool canLight;
	string nameLit;
	string nameOut;
	string nameDead;
};

class Matchbook: public Object{
public:
	Matchbook(string n = "matchbook", double w = 1, double s = 1, string d = "matchbook", int numMatch = 10, bool open = false) {
		weight = w;
		size = s;
		description = d;
		numMatches = numMatch;
		isOpen = open;
		for (int i = 0; i < numMatches; i++) {
			matches.push_back(new Light("match", .1, .1, "There is nothing special about the match", MATCH_LIFE, true, true));
		}
		nameOpen = n + " (open)";
		nameClosed = n + " (closed)";
		if (open) {
			name = nameOpen;
		}
		else {
			name = nameClosed;
		}
	}

	bool isLitCanLight(Object* o) override {
		if (splitString(o->getName(), ' ')[0] == "match" && o->time_til_dead() == MATCH_LIFE) {
			return true;
		}
		return false;
	}
	bool takeOut(Object* thingToTakeOut) override{
		matches.erase(matches.begin());
		numMatches--;
		return true;
	}
	vector<Object*> getInventory() override{
		if (isOpen) {
			return matches;
		}
		else {
			return vector<Object*> {};
		}
	}
	bool is_open() override{
		return isOpen;
	}
	bool open() override{
		isOpen = true;
		name = nameOpen;
		return true;
	}
	bool close() override {
		isOpen = false;
		name = nameClosed;
		return true;
	}

	string getDescription() override{
		string des = description + "\n";
		if (isOpen) {
			des += "The " + splitString(name, ' ')[0] + " contains:";

			des += "\n " + to_string(numMatches) + " matches\n";
		}
		else {
			des += "The " + splitString(name, ' ')[0] + " is closed.";
		}

		return des;
	}

	string printInventory(string tabs) override{
		string des = "";
		if (isOpen) {
			des += "\n" + tabs + "The " + splitString(name, ' ')[0] + " contains:";

			des += "\n " + tabs + "\t" + to_string(numMatches) + " matches\n";
		}
		return des;
	}
private:
	int numMatches;
	vector<Object*> matches = {};
	bool isOpen;
	string nameOpen;
	string nameClosed;
};

#endif
