#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include<string>
#include<vector>
#include<iostream>
#include "map.h"
#include "map.hpp"
#include "crossing.hpp"
#include "fileOperations.h"


TEST_CASE("Find by name")
{
    FileOperations f;
    std::vector<Crossing*> crossings = f.read("test.txt");
    CHECK(f.find(crossings, "p") != nullptr);
}

TEST_CASE("Loading from file")
{
    std::string filename = "test.txt";
    FileOperations f;
    std::vector<Crossing*> crossings = f.read(filename);
    CHECK(crossings.size() == 4);
    CHECK(crossings[0]->getId() == 0);
    CHECK(crossings[1]->getId() == 1);
    CHECK(crossings[2]->getId() == 2);
    CHECK(crossings[3]->getId() == 3);
    CHECK(crossings[0]->getName() == "p");
    CHECK(crossings[1]->getName() == "b");
    CHECK(crossings[2]->getName() == "k");
    CHECK(crossings[3]->getName() == "n");
    CHECK(crossings[0]->streetsCount() == 2);
}

TEST_CASE("Is connected")
{
    Map* map = new Map("connected.txt");
    CHECK(map->isConnected());

    Map* notCon = new Map("notConnected.txt");
    CHECK(!notCon->isConnected());

    Map* eulerian = new Map("eulerian.txt");
    CHECK(eulerian->isConnected());
}

TEST_CASE("Copy")
{
    Map* map = new Map("connected.txt");
    Map* copy = new Map(*map);
    CHECK(copy->isConnected());
    CHECK(copy->getStreetsCount() == map->getStreetsCount());
}

TEST_CASE("Dead-end streets")
{
    Map* map = new Map("notConnected.txt");
    std::vector<DEStreet*> des = map->deadEndStreets();
    CHECK(des.size() == 1);
}

TEST_CASE("Reachable")
{
    Map* map = new Map("connected.txt");
    CHECK(map->allReachableFrom(1));
    Map* notReachable = new Map("notConnected.txt");
    CHECK(!notReachable->allReachableFrom(2));
}

TEST_CASE("Route between")
{
    Map* map = new Map("test.txt");
    FileOperations f;
    std::vector<Crossing*> crossings = f.read("test.txt");
    CHECK(map->HasPathBetween(crossings[0], crossings[3]));
}

TEST_CASE("Has Circuit")
{
    Map* map = new Map("test.txt");
    FileOperations f;
    std::vector<Crossing*> crossings = f.read("test.txt");
    CHECK(map->HasCircuit(crossings[0]));
}

TEST_CASE("Has eulerian circuit")
{
    Map* map = new Map("eulerian.txt");
    CHECK(map->isConnected());
    CHECK(map->hasEulerianCircuit());
}

TEST_CASE("Euler circuit")
{
    Map* map = new Map("eulerian.txt");
    std::vector<Crossing*> circuit = map->eulerianCircuit();
    CHECK(circuit.size() != 0);
    std::cout << "Euler circuit: "<<std::endl;
    for(auto cr : circuit)
    {
        std::cout << cr->getName()<<" -> ";
    }
    std::cout<<std::endl;
}

TEST_CASE("Shortest paths")
{
    FileOperations f;
    std::vector<Crossing*> crossings = f.read("paths.txt");
    Map* map = new Map("paths.txt");
    std::vector<Crossing*>* paths = map->ThreeShortestPaths(crossings[0], crossings[3]);
    CHECK(paths[0].size() > 0);
    CHECK(paths[1].size() > 0);

    std::cout << "Shortest paths: "<<std::endl;
    for(int i = 0; i < 3; i++)
    {
        std::vector<Crossing*> path = paths[i];
        std::vector<Crossing*>::iterator it = path.begin();
        for(it; it != path.end(); it++)
        {
            std::cout << (*it)->getName() <<"->";
        }
        std::cout<<std::endl;
    }
}

int main ()
{
    doctest::Context().run();
    return 0;
}