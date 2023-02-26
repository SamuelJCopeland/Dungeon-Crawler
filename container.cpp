#include <algorithm>

#include "container.h"
#include "parser.h"

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: container.cpp
*/

Container::Container(std::string aName, double aWeight, double aSize, std::string aDescription, double aCapacity, bool aIsOpen, std::vector<Object*> aInventory)
    : Object(aName, aWeight, aSize, aDescription)
    , mSizeCapacity(aCapacity)
    , mIsOpen(aIsOpen)
    , mNameOpen(aName + " (open)")
    , mNameClosed(aName + " (closed)")
{

    for (Object* o : aInventory) 
    {

        if (mSizeCapacity - o->size() - o->internalSize() >= 0) 
        {
            mInternalSize += o->size() + o->internalSize();
            mSizeCapacity -= o->size();
            mInventory.push_back(o);
        }

    }

    if (aIsOpen) 
    {
        mName = mNameOpen;
    }
    else 
    {
        mName = mNameClosed;
    }

}

std::string Container::description()
{
    std::string des = mDescription + "\n";

    if (mIsOpen)
    {
        des += "The " + Parser::split(mName, " ")[0] + " contains:";

        for (Object* o : mInventory)
        {
            des += "\n\t" + o->name();

            if (o->isOpen() && o->inventory().size() > 0)
            {
                des += o->printInventory("\t") + "\n";
            }

        }

    }
    else
    {
        des += "The " + Parser::split(mName, " ")[0] + " is closed.";
    }

    return des;
}

double Container::capacity()
{
    return 0.0;
}

double Container::internalSize()
{
    return mInternalSize;
}

std::vector<Object*> Container::inventory()
{

    if (mIsOpen)
    {
        return mInventory;
    }
    else
    {
        return std::vector<Object*> {};
    }

}

bool Container::open()
{
    mIsOpen = true;
    mName = mNameOpen;
    return true;
}

bool Container::close()
{
    mIsOpen = false;
    mName = mNameClosed;
    return true;
}

bool Container::isOpen()
{
    return mIsOpen;
}

bool Container::addToContainer(Object* c)
{
    if (c->addSize(this))
    {

        for (Object* o : mInventory)
        {
            o->addToContainer(c);
            c->subSize(o);
        }

        inContainers.push_back(c);
        return true;
    }
    else
    {
        return false;
    }

}

void Container::leaveContainers()
{

    for (int i = 0; i < inContainers.size(); i++)
    {
        inContainers[i]->subSize(this);
    }

    inContainers = {};
}

bool Container::addObject(Object* o)
{

    if (mIsOpen && o != this)
    {
        bool canAdd = false;

        for (Object* c : inContainers)
        {
            bool test = o->addToContainer(c);

            if (test)
            {
                canAdd = true;
            }
        }

        if (canAdd || inContainers.size() == 0)
        {

            if (o->addToContainer(this))
            {
                mInventory.push_back(o);
                return true;
            }
            else
            {
                return false;
            }

        }
        else
        {
            o->leaveContainers();
            return false;
        }

    }

    return false;
}

bool Container::removeObject(Object* aObject)
{
    std::vector<Object*>::iterator position = find(mInventory.begin(), mInventory.end(), aObject);

    if (position != mInventory.end() && mIsOpen)
    {
        mInventory.erase(position);
        aObject->leaveContainers();
        return true;
    }

    return false;
}

void Container::subSize(Object* o)
{
    mInternalSize -= (o->size() + o->internalSize());
    mSizeCapacity += o->size() + o->internalSize();
    mWeight -= o->weight();
}

bool Container::addSize(Object* o)
{

    if (mSizeCapacity - (o->size() + o->internalSize()) >= 0)
    {
        mSizeCapacity -= (o->size() + o->internalSize());
        mWeight += o->weight();
        mInternalSize += (o->size() + o->internalSize());
        return true;
    }
    else
    {
        return false;
    }

}

std::string Container::printInventory(std::string tabs)
{
    std::string des = "";
    des += "\n" + tabs + "The " + Parser::split(mName, " ")[0] + " contains:";

    for (Object* o : mInventory)
    {
        des += "\n" + tabs + "\t" + o->name();

        if (o->isOpen() && o->inventory().size() > 0)
        {
            des += o->printInventory(tabs + "\t");
        }

    }
    return des;
}
