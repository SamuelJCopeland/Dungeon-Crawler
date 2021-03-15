#include "container.h"
#include <algorithm>

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: container.cpp
*/

using namespace std;

string Container::getDescription(){
    string des = description + "\n";
    if (isOpen) {
        des += "The " + splitString(name, ' ')[0] + " contains:";

        for (Object* o : inventory) {
            des += "\n\t" + o->getName();
            if (o->is_open() && o->getInventory().size() > 0) {
                des += o->printInventory("\t") + "\n";
            }
        }
    }
    else {
        des += "The " + splitString(name, ' ')[0] + " is closed.";
    }

    return des;
}

string Container::printInventory(string tabs) {
    string des = "";
    des += "\n" + tabs + "The " + splitString(name, ' ')[0] + " contains:";

    for (Object* o : inventory) {
        des += "\n" + tabs + "\t" + o->getName();
        if (o->is_open() && o->getInventory().size() > 0) {
            des += o->printInventory(tabs + "\t");
        }
    }
    return des;
}

double Container::getCapacity()
{
    return 0.0;
}

bool Container::putIn(Object* o){
    if (isOpen && o != this) {
        bool canAdd = false;
        for (Object* c : inContainers) {
            bool test = o->addContainer(c);
            if (test) {
                canAdd = true;
            }
        }
        if (canAdd || inContainers.size() == 0) {
            if (o->addContainer(this)) {
                inventory.push_back(o);
                return true;
            }
            else {
                return false;
            }
        }
        else {
            o->leaveContainers();
            return false;
        }
    }
    return false;
}

bool Container::takeOut(Object* o){
    vector<Object*>::iterator position = find(inventory.begin(), inventory.end(), o);
    if (position != inventory.end() && isOpen) {
        inventory.erase(position);
        o->leaveContainers();
        return true;
    }
    return false;
}

double Container::getInternalSize(){
    return internalSize;
}
