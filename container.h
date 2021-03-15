#include"object.h"
#include <vector>

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: container.h
*/

using namespace std;

#ifndef CONTAINER_H
#define CONTAINER_H

class Container : public Object {
public:
	Container(string n = "generic_container", double w = 1, double s = 1, string d = "There is nothing special about the generic_container.", double c = 10, bool o = false, vector<Object*> i = {}) {
		weight = w;
		size = s;
		sizeCapacity = c;
		isOpen = o;
		description = d;

		for (Object* o : i) {
			if (sizeCapacity - o->getSize() - o->getInternalSize() >= 0) {
				internalSize += o->getSize() + o->getInternalSize();
				sizeCapacity -= o->getSize();
				inventory.push_back(o);
			}
		}
		nameOpen = n + " (open)";
		nameClosed = n + " (closed)";
		if(o) {
			name = nameOpen;
		}
		else {
			name = nameClosed;
		}
	}

	bool addContainer(Object* c) override{
		if (c->addSize(this)) {
			for (Object* o : inventory) {
				o->addContainer(c);
				c->subSize(o);
			}
			inContainers.push_back(c);
			return true;
		}
		else {
			return false;
		}
	}

	void leaveContainers() override{
		for (int i = 0; i < inContainers.size(); i++) {
			inContainers[i]->subSize(this);
		}
		inContainers = {};
	}

	string getDescription() override;
	double getCapacity() override;
	bool putIn(Object* thingToPutIn) override;
	bool takeOut(Object* thingToTakeOut) override;
	double getInternalSize() override;
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
	vector<Object*> getInventory() override{
		if (isOpen) {
			return inventory;
		}
		else {
			return vector<Object*> {};
		}
	}
	void subSize(Object* o) override{
		internalSize -= (o->getSize() + o->getInternalSize());
		sizeCapacity += o->getSize() + o->getInternalSize();
		weight -= o->getWeight();
	}

	bool addSize(Object* o) override{
		if (sizeCapacity - (o->getSize() + o->getInternalSize()) >= 0) {
			sizeCapacity -= (o->getSize() + o->getInternalSize());
			weight += o->getWeight();
			internalSize += (o->getSize() + o->getInternalSize());
			return true;
		}
		else {
			return false;
		}
	}

	bool is_open() override{
		return isOpen;
	}

	string printInventory(string tabs) override;

	
private:
	bool isOpen;
	vector<Object*> inventory = {};
	double sizeCapacity;
	double internalSize = 0;
	string nameOpen;
	string nameClosed;
};

#endif