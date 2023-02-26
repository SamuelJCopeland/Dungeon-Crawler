#ifndef WEAPON_H
#define WEAPON_H
/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: weapon.h
*/

#include <string>

#include "object.h"

class Weapon : public Object 
{
public:

	Weapon(std::string aName, double aWeight, double aSize, std::string aDescription, double aDamage);

	double damage() override;

private:

	double mDamage;
};


#endif
