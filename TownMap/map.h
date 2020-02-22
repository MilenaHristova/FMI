#ifndef MAP_H
#define MAP_H

#include<list>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
#include<functional>
#include "crossing.hpp"
#include "fileOperations.hpp"

struct DEStreet
{
    std::string start;
    std::string end;
    DEStreet(std::string _start, std::string _end):start(_start), end(_end){}
};

class Map
{
    FileOperations fileOperations;
    int crossingsCount;
    int streetsCount;
    std::vector<Crossing*> crossings;
    std::vector<Crossing*> shortestPaths[3];
    
    int* shortestLengths;
    int shortestPathsCount;
    bool contains(std::vector<Crossing*> vect, Crossing* crossing);
    Crossing* findById(std::vector<Crossing*> vect, int id);
    void FindShortestPaths(Crossing* start, Crossing* end, bool* visited, int length,
                            std::vector<Crossing*> path,
                            std::vector<Crossing*>& closed);
    void UpdateShortestPaths(std::vector<Crossing*>& path, int length);
    void DFS(Crossing* start, bool* visited);
    int* getIndegrees();
    std::vector<Crossing*> deadEndStreetsFrom(Crossing* start);

    public:
    Map();
    Map(std::string filename);
    Map(std::vector<Crossing*>& crossings);
    Map(const Map& map);
    ~Map();
    int getStreetsCount();
    bool HasPathBetween(Crossing* first, Crossing* second)const;
    std::vector<Crossing*>* ThreeShortestPaths(Crossing* first, Crossing* second);
    std::vector<Crossing*>* ThreeShortestPaths(Crossing* first, Crossing* second,
                            std::vector<Crossing*>& closed);
    bool HasCircuit(Crossing* start);
    std::vector<Crossing*> eulerianCircuit(); 
    bool isConnected();
    bool hasEulerianCircuit();
    Map* transpose();
    bool allReachableFrom(Crossing* start);
    bool allReachableFrom(int id);
    std::vector<DEStreet*> deadEndStreets();
};

#endif