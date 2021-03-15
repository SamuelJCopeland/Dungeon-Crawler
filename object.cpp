#include "object.h"

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: object.cpp
*/

using namespace std;

bool Object::light(Object* lightingObject){
	//cout << "You can't light " << name << " with " << lightingObject->getName() << "\n";
	return false;
}

bool Object::putOut(){
	//cout << "The " << name << " is not lit\n";
	return false;
}

double Object::getDamage(){
	return 0;
}

string Object::getDescription(){
	return description;
}

double Object::getCapacity(){
	return 0;
}

bool Object::putIn(Object* thingToPutIn){
	return false;
}

bool Object::takeOut(Object* thingToTakeOut){
	return false;
}

string Object::getName()
{
	return name;
}

double Object::getWeight()
{
	return weight;
}

double Object::getSize()
{
	return size;
}