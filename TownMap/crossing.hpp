#ifndef CROSSING_HPP
#define CROSSING_HPP
#include "crossing.h"
#include <list>
#include <cassert>

int Crossing::getId() const
{
    return id;
}

std::string Crossing::getName()
{
    return name;
}

bool Crossing::operator==(const Crossing& other)const
{
    return this->id == other.getId();
}

std::map<Crossing*, int>::const_iterator Crossing::begin()
{
    return streets.begin();
}

std::map<Crossing*, int>::const_iterator Crossing::end()
{
    return streets.end();
}

int Crossing::streetsCount()
{
    return streets.size();
}

void Crossing::addStreet(Crossing* next, int length)
{
    assert(next != nullptr);
    streets.insert(std::pair<Crossing*, int>(next, length));
}

void Crossing::deleteStreet(Crossing* next)
{
    assert(next != nullptr);
    streets.erase(next);
}



#endif
