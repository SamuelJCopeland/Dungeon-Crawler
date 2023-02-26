#include <string>
#include <vector>

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: object.h
*/

#ifndef OBJECT_H
#define OBJECT_H

class Player;

class Object
{
public:

	// Object stuff
	Object(std::string aName = "generic_object", double aWeight = 1, double aSize = 1, std::string aDescription = "There is nothing special about the generic_object.");

	virtual std::string name();

	virtual std::string description();

	double weight();

	double size();

	virtual bool addToContainer(Object* aContainer);

	virtual void leaveContainers();

	// Container stuff
	virtual double capacity();

	virtual std::vector<Object*> inventory();

	virtual double internalSize();

	virtual int timeTilDead();

	virtual bool isOpen();

	virtual bool open();

	virtual bool close();

	virtual bool addObject(Object* aObject);

	virtual bool removeObject(Object* aObject);

	virtual bool addSize(Object* aObject);

	virtual void subSize(Object* aObject);

	virtual std::string printInventory(std::string aIndent);

	// Weapon stuff
	virtual double damage();

	// Light stuff
	virtual bool isLitCanLight(Object* aO);

	virtual bool light(Object * aLightingObject);

	virtual bool putOut();

	virtual bool updateLight();

	// Food stuff
	virtual bool eat(Player * aPlayer);

	// Helper functions
	static Object* findObject(std::string aName, std::vector<Object*> aObject);

	static std::vector<Object*> recursiveFindObjects(std::vector<Object*> aObject);

	static Object* recursiveFindContainer(Object * aFindOb, Object * aContainingObject);

	static Object* recursiveFindByName(std::string aName, std::vector<Object*> aContainingObject);

protected:

	//The name of the object
	std::string mName;

	//How heavy the object is
	double mWeight;

	//How big the object is
	double mSize;

	std::string mDescription;

	std::vector<Object*> inContainers = {};
};

#endif
