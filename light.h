/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: light.h
*/

#ifndef LIGHT_H
#define LIGHT_H

#include <string>
#include <vector>

#include "object.h"
#include "parser.h"

class Light : public Object {
public:
	Light(std::string aName = "generic_light", double aWeight = 1, double aSize = 1, std::string aDescription = "There is nothing special about the generic_light.", int aTimeTilDead = 1000, bool aNeedsLighter = false, bool aCanLight = false);

	bool light(Object* lightingObject) override;

	bool putOut() override;

	bool updateLight() override;

	bool isLitCanLight(Object* o) override;

	int timeTilDead();

private:

	int mTimeTilDead;
	bool mNeedsLighter;
	bool mIsLit = false;
	bool mCanLight;
	std::string mNameLit;
	std::string mNameOut;
	std::string mNameDead;
};

#endif
