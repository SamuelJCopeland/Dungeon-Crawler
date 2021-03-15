#ifndef WEAPON_H
#define WEAPON_H

#include "object.h"

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: weapon.h
*/

using namespace std;

class Weapon : public Object {
public:
	Weapon(string n = "generic_weapon", double w = 1, double s = 1, string d = "There is nothing special about the generic_weapon.", double dam = 5) {
		name = n;
		weight = w;
		size = s;
		description = d;
		damage = dam;
	}
	double getDamage() override{
		return damage;
	}
private:
	double damage;
};


#endif
