#include "player.h"
#include <time.h>
#include <iostream>
#include <algorithm>

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: player.cpp
*/

vector<Object*> Player::attack(Player* enemy, Object* weapon){
	double damage = weapon->getDamage();
	if (damage > 0) {
		//Total damage = base weapon damage * strength + d4
		damage = damage * strength + rand() % 4 + 1;
		return enemy->takeHit(this, damage);
	}
	else {
		cout << "You can't attack the " + enemy->getName() + " with " + weapon->getName() + ".\n";
	}
}

vector<Object*> Player::takeHit(Player* enemy, double damage) {
	int dodgeChance = this->agility;
	if (dodgeChance < 0){
		dodgeChance = 0;
	}
	srand(time(NULL));
	
	int damageChance = rand() % MAX_AGILITY + 1;

	if (damageChance > dodgeChance) {
		hitpoints -= damage;
		if (hitpoints <= 0) {
			cout << name << " " << "died.\n";
			alive = false;
			vector<Object*> temp = inventory;
			inventory = vector<Object*>();
			return temp;
		}
		else {
			cout << enemy->getName() << " hit " << name << ".\n";
		}
	}
	else {
		cout << name << " dodged the attack.\n";
	}
	return vector<Object*>{};
}

bool Player::removeObject(Object* o){
	vector<Object*>::iterator position = find(inventory.begin(), inventory.end(), o);
	if (position != inventory.end()) {
		inventory.erase(position);
		weightCapacity += o->getWeight();
		sizeCapacity += o->getSize();
		return true;
	}
	return false;
}

bool Player::addObject(Object* o){
	if (weightCapacity - o->getWeight() < 0 || sizeCapacity - o->getSize() < 0) {
		return false;
	}
	else {
		weightCapacity -= o->getWeight();
		sizeCapacity -= o->getSize() < 0;
		inventory.push_back(o);
		return true;
	}
}

string Player::printInventory()
{
	string iDescription = "";
	
	for (Object* o : inventory) {
		iDescription += "\t" + o->getName();
		if (o->is_open() && o->getInventory().size() > 0) {
			iDescription += o->printInventory("\t") + "\n";
		}
		else {
			iDescription += "\n";
		}
	}

	return iDescription;
}

string Player::getName(){
	return name;
}

double Player::getHP(){
	return hitpoints;
}

double Player::getAgility(){
	return agility;
}

Player* findPlayer(string name, vector<Player*> pl){
	for (Player* p : pl) {
		string pName = p->getName();
		size_t found = pName.find(" (");


		if (pName == name) {
			return p;
		}
	}
	return nullptr;
}
