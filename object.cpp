#include "object.h"

#include <string>
#include <vector>
#include <iostream>

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: object.cpp
*/

// Object stuff
Object::Object(std::string aName, double aWeight, double aSize, std::string aDescription)
	: mName(aName)
	, mWeight(aWeight)
	, mSize(aSize)
	, mDescription(aDescription)
{}

std::string Object::name()
{
	return mName;
}

std::string Object::description()
{
	return mDescription;
}

double Object::weight()
{
	return mWeight;
}

double Object::size()
{
	return mSize;
}

bool Object::addToContainer(Object* aContainer)
{

	if (aContainer->addSize(this))
	{
		inContainers.push_back(aContainer);
		return true;
	}
	else
	{
		return false;
	}

}

void Object::leaveContainers()
{

	for (int i = 0; i < inContainers.size(); i++)
	{
		inContainers[i]->subSize(this);
	}

	inContainers = {};
}

// Container stuff
double Object::capacity()
{
	return 0;
}

std::vector<Object*> Object::inventory()
{
	return std::vector<Object*>();
}

double Object::internalSize()
{
	return 0;
}

int Object::timeTilDead()
{
	return 10000;
}

bool Object::isOpen()
{
	return false;
}

bool Object::open()
{
	return false;
}

bool Object::close()
{
	return false;
}

bool Object::addObject(Object* aObject)
{
	return false;
}

bool Object::removeObject(Object* aObject)
{
	return false;
}

bool Object::addSize(Object* aObject)
{
	return false;
}

void Object::subSize(Object* aObject)
{}

std::string Object::printInventory(std::string aIndent)
{
	return "";
}

// Weapon stuff
double Object::damage()
{
	return 0;
}

// Light stuff
bool Object::isLitCanLight(Object* aO)
{
	return false;
}

bool Object::light(Object* aLightingObject)
{
	return false;
}

bool Object::putOut()
{
	//cout << "The " << name << " is not lit\n";
	return false;
}

bool Object::updateLight()
{
	return false;
}

// Food stuff
bool Object::eat(Player* aPlayer)
{
	return false;
}

// Helper functions
Object* Object::findObject(std::string aName, std::vector<Object*> aObject) 
{

	for (Object* o : aObject) 
	{
		std::string oName = o->name();
		size_t found = oName.find(" (");

		if (found != std::string::npos) 
		{
			oName = oName.substr(0, (found - 0));
		}

		if (oName == aName) 
		{
			return o;
		}

	}

	return nullptr;
}

std::vector<Object*> Object::recursiveFindObjects(std::vector<Object*> aObject) 
{
	std::vector<Object*> objects = {};

	for (Object* o : aObject) 
	{
		objects.push_back(o);

		if (o->isOpen()) 
		{

			for (Object* obj : recursiveFindObjects(o->inventory())) 
			{
				objects.push_back(obj);
			}
		}
		else if (o->open()) 
		{
			o->close();
		}

	}

	return objects;
}

Object* Object::recursiveFindContainer(Object * aFindOb, Object * aContainingObject) 
{
	Object* container = nullptr;

	for (Object* ob : aContainingObject->inventory()) 
	{

		if (ob == aFindOb) 
		{
			return aContainingObject;
		}
		else 
		{
			container = recursiveFindContainer(aFindOb, ob);
		}

	}

	return container;
}

Object* Object::recursiveFindByName(std::string aName, std::vector<Object*> aContainingObject) 
{
	Object* result = nullptr;

	for (Object* ob : aContainingObject) 
	{
		result = recursiveFindByName(aName, ob->inventory());

		if (result != nullptr) 
		{
			return result;
		}

	}

	if (result == nullptr) 
	{
		result = findObject(aName, aContainingObject);
	}

	if (result != nullptr) 
	{
		return result;
	}

	return result;
}