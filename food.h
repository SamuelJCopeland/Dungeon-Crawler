/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: food.h
*/

#ifndef FOOD_H
#define FOOD_H

#include <string>

#include "object.h"
#include "player.h"

class Food : public Object
{
public:
	Food(std::string aName = "generic_food", double aWeight = 1, double aSize = 1, std::string aDescription = "There is nothing special about the generic_food.", double aHealthRestoration = 5);

	bool eat(Player * aPlayer) override;

private:
	double mHealthRestoration;
};

#endif
