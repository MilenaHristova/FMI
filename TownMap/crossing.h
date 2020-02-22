#ifndef CROSSING_H
#define CROSSING_H

#include<vector>
#include<string>
#include<map>

class Street;
class Crossing
{
    int id;
    std::string name;
    std::map<Crossing*, int> streets;

    public:
    Crossing(std::string _name, std::map<Crossing*, int> _streets) : name(_name), streets(_streets){}
    Crossing(std::string _name, int _id) : name(_name), id(_id){}
    int getId()const;
    std::string getName();
    bool operator==(const Crossing& other)const;
    std::map<Crossing*, int>::const_iterator begin();
    std::map<Crossing*, int>::const_iterator end();
    int streetsCount();
    void addStreet(Crossing* next, int length);
    void deleteStreet(Crossing* next);
    friend class FileOperations;
};

#endif