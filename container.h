/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: container.h
*/

#ifndef CONTAINER_H
#define CONTAINER_H

#include"object.h"
#include <vector>

class Container : public Object {
public:
	Container(std::string aName = "generic_container", double aWeight = 1, double aSize = 1, std::string aDescription = "There is nothing special about the generic_container.", double aCapacity = 10, bool aIsOpen = false, std::vector<Object*> aInventory = {});

	std::string description() override;

	double capacity() override;

	double internalSize() override;

	std::vector<Object*> inventory() override;

	bool open() override;

	bool close() override;

	bool isOpen() override;

	bool addToContainer(Object* c) override;

	void leaveContainers() override;

	bool addObject(Object* thingToPutIn) override;

	bool removeObject(Object * aObject) override;

	void subSize(Object* o) override;

	bool addSize(Object* o) override;

	std::string printInventory(std::string tabs) override;

	
protected:
	bool mIsOpen;
	std::vector<Object*> mInventory = {};
	double mSizeCapacity;
	double mInternalSize = 0;
	std::string mNameOpen;
	std::string mNameClosed;
};

#endif