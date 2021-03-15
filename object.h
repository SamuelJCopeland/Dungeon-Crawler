#include <string>
#include <iostream>
#include <vector>

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: object.h
*/

using namespace std;

#ifndef OBJECT_H
#define OBJECT_H
class Player;

class Object {
public:
	Object(string n = "generic_object", double w = 1, double s = 1, string d = "There is nothing special about the generic_object.") {
		name = n;
		weight = w;
		size = s;
		description = d;
	}
	virtual bool light(Object* lightingObject);
	virtual bool updateLight() {
		return false;
	}
	virtual bool eat(Player* p) {
		return false;
	}
	virtual bool isLitCanLight(Object* o) {
		return false;
	}
	virtual bool putOut();
	virtual double getDamage();
	virtual string getDescription();
	virtual double getCapacity();
	virtual bool putIn(Object* thingToPutIn);
	virtual bool takeOut(Object* thingToTakeOut);
	virtual vector<Object*> getInventory() {
		return vector<Object*> {};
	}
	virtual bool is_open() {
		return false;
	}
	virtual double getInternalSize() {
		return 0;
	}
virtual bool open() {
	return false;
}
virtual int time_til_dead() {
	return 10000;
}
virtual bool close() {
	return false;
}
virtual string getName();
virtual string printInventory(string tabs) {
	return "";
}
virtual void subSize(Object* o) {}

virtual void leaveContainers() {
	for (int i = 0; i < inContainers.size(); i++) {
		inContainers[i]->subSize(this);
	}
	inContainers = {};
}
virtual bool addSize(Object* o) {
	return false;
}
virtual bool addContainer(Object* c) {
	if (c->addSize(this)) {
		inContainers.push_back(c);
		return true;
	}
	else {
		return false;
	}
}
double getWeight();
double getSize();
protected:
	//The name of the object
	string name;
	//How heavy the object is
	double weight;
	//How big the object is
	double size;
	string description;
	vector<Object*> inContainers = {};
};

static Object* findObject(string name, vector<Object*> ob) {
	for (Object* o : ob) {
		string oName = o->getName();
		size_t found = oName.find(" (");
		if (found != string::npos) {
			oName = oName.substr(0, (found - 0));
		}

		if (oName == name) {
			return o;
		}
	}
	return nullptr;
}

static vector<Object*> recursiveFindObjects(vector<Object*> ob) {
	vector<Object*> objects = {};
	for (Object* o : ob) {
		objects.push_back(o);
		if (o->is_open()) {
			for (Object* obj : recursiveFindObjects(o->getInventory())) {
				objects.push_back(obj);
			}
		}
		else if (o->open()) {
			o->close();
		}
	}
	return objects;
}

//Function to split a string based on a char
static vector<string> splitString(string s, char c) {
	vector<string> stringV = {};
	string temp = "";
	for (char ch : s) {
		if (ch == c) {
			stringV.push_back(temp);
			temp = "";
		}
		else {
			temp += ch;
		}
	}
	stringV.push_back(temp);
	return stringV;
}

static Object* recursiveFindContainer(Object* findOb, Object* o) {
	Object* container = nullptr;
	for (Object* ob : o->getInventory()) {

		if (ob == findOb) {
			return o;
		}
		else {
			container = recursiveFindContainer(findOb, ob);
		}
	}
	return container;
}

static Object* recursiveFindByName(string n, vector<Object*> o) {
	Object* result = nullptr;
	for (Object* ob : o) {
		result = recursiveFindByName(n, ob->getInventory());
		if (result != nullptr) {
			return result;
		}
	}

	if (result == nullptr) {
		result = findObject(n, o);
	}

	if (result != nullptr){
		return result;
	}
	return result;
}

#endif
