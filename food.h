#include "object.h"

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: food.h
*/

using namespace std;

#ifndef FOOD_H
#define FOOD_H
class Food : public Object {
public:
	Food(string n = "generic_food", double w = 1, double s = 1, string d = "There is nothing special about the generic_food.", double r = 5) {
		name = n;
		weight = w;
		size = s;
		description = d;
		healthRestoration = r;
	}

	bool eat(Player* p) override {
		if (p->getHP() <= p->getMaxHealth()) {
			double temp = p->getHP();
			temp += healthRestoration;
			if (temp > p->getMaxHealth()) {
				temp = p->getMaxHealth();
			}
			p->setHP(temp);
		}
		return true;
	}
private:
	double healthRestoration;
};
#endif
