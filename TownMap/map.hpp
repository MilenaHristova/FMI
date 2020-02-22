#ifndef MAP_HPP
#define MAP_HPP
#include "Map.h"
#include <iostream>
#include <algorithm>
#include <stack>

Map::Map()
{
    shortestLengths = new int[3];
    shortestPathsCount = 0;
}

Map::Map(std::string filename):Map()
{
    crossings = fileOperations.read(filename);
    crossingsCount = crossings.size();
    streetsCount = getStreetsCount();
}

Map::Map(const Map& map)
{
    std::vector<Crossing*> _crossings;
    int _streetsCount = 0;
    for(auto crossing: map.crossings)
    {
        Crossing* current = findById(_crossings, crossing->getId());
        if(current == nullptr)
        {
            current = new Crossing(crossing->getName(), crossing->getId());
            _crossings.push_back(current);
        }
        for(auto street : *crossing)
        {
            Crossing* next = street.first;
            Crossing* copyNext = findById(_crossings, next->getId());
            if(copyNext == nullptr)
            {
                copyNext = new Crossing(next->getName(), next->getId());
                _crossings.push_back(copyNext);
            }
            current->addStreet(copyNext, street.second);
            _streetsCount++;
        }
    }
    crossings = _crossings;
    streetsCount = _streetsCount;
    crossingsCount = _crossings.size();
}

Map::Map(std::vector<Crossing*>& _crossings):Map()
{
    crossings = _crossings;
    crossingsCount = crossings.size();
    streetsCount = getStreetsCount();
}

Map::~Map() 
{
    delete[] shortestLengths;
}

int Map::getStreetsCount()
{
    int count = 0;
    std::vector<Crossing*>::iterator it;
    for(it = crossings.begin(); it != crossings.end(); it++)
    {
        count += (*it)->streetsCount();
    }
    return count;
}

bool Map::contains(std::vector<Crossing*> vect, Crossing* crossing)
{
    std::vector<Crossing*>::iterator it;
    for(it = vect.begin(); it != vect.end(); it++)
    {
        if((*it)->getId() == crossing->getId())return true;
    }
    return false;
}

Crossing* Map::findById(std::vector<Crossing*> vect, int id)
{
    for(auto crossing : vect)
    {
        if(crossing->getId() == id)
        {
            return crossing;
        }
    }
    return nullptr;
}

bool Map::isConnected()
{
    bool* visited = new bool[crossingsCount];
    memset(visited, false, crossingsCount);

    Crossing* start;
    std::vector<Crossing*>::iterator it;
    for(it = crossings.begin(); it != crossings.end(); it++)
    {
        if((*it)->streetsCount() > 0)
        {
            start = *it;
            break;
        }
    }

    DFS(start, visited);

    for(int i = 0; i < crossingsCount; i++)
    {
        if(visited[i] == false && crossings[i]->streetsCount() > 0)
        {
            delete[] visited;
            return false;
        }
    }

    Map* transposed = transpose();
    memset(visited, false, crossingsCount);

    DFS(transposed->crossings[0], visited);
    for(int i = 0; i < crossingsCount; i++)
    {
        if(visited[i] == false && crossings[i]->streetsCount() > 0)
        {
            delete[] visited;
            return false;
        }
    }

    delete[] visited;
    return true;
}

void Map::DFS(Crossing* start, bool* visited)
{
    visited[start->getId()] = true;
    for(auto street : *start)
    {
        Crossing* next = street.first;
        if(!visited[next->getId()])
        {  
            DFS(next, visited);
        }
    }
}

Map* Map::transpose()
{
    std::vector<Crossing*> transposedCrossings;
    for(auto crossing:crossings)
    {
        Crossing* current = findById(transposedCrossings, crossing->getId());
        if(current == nullptr)
        {
            current = new Crossing(crossing->getName(), crossing->getId());
            transposedCrossings.push_back(current);
        }
        for(auto street:*crossing)
        {
            Crossing* next = street.first;
            Crossing* transposedNext = findById(transposedCrossings, next->getId());
            if(transposedNext == nullptr)
            {
                transposedNext = new Crossing(next->getName(), next->getId());
                transposedCrossings.push_back(transposedNext);
            }
            transposedNext->addStreet(current, street.second);
        }
    }
    return new Map(transposedCrossings);
}

bool Map::HasPathBetween(Crossing* first, Crossing* second)const
{
    if(first->getId() == second->getId()) return true;

    bool *visited = new bool[crossingsCount];
    for(int i = 0; i < crossingsCount; i++)
    {
        visited[i] = false;
    }

    std::list<Crossing*> queue;

    visited[first->getId()] = true;
    queue.push_back(first);

    std::map<Crossing*, int>::const_iterator it;

    while(!queue.empty())
    {
        Crossing* current = queue.front();
        queue.pop_front();
        for(it = current->begin(); it != current->end(); it++)
        {
            if((*it).first->getId() == second->getId())
            {
                delete[] visited;
                return true;
            } 
            if(!visited[(*it).first->getId()])
            {
                visited[(*it).first->getId()] = true;
                queue.push_back((*it).first);
            }
        }
    }
    delete[] visited;
    return false;   
}

std::vector<Crossing*>* Map::ThreeShortestPaths(Crossing* first, Crossing* second)
{
    std::vector<Crossing*> cl;
    return ThreeShortestPaths(first, second, cl);
}

std::vector<Crossing*>* Map::ThreeShortestPaths(Crossing* first, Crossing* second, std::vector<Crossing*>& closed)
{
    std::vector<Crossing*> path;

    bool *visited = new bool[crossingsCount];
    for(int i = 0; i < crossingsCount; i++)
    {
        visited[i] = false;
    }

    FindShortestPaths(first, second, visited, 0, path, closed);

    delete[] visited;
    return shortestPaths;
}

void Map::FindShortestPaths(Crossing* start, Crossing* end, bool* visited, int length,
                                std::vector<Crossing*> path,
                                std::vector<Crossing*>& closed)
{
    visited[start->getId()] = true;
    path.push_back(start);

    if(start->getId() == end->getId())
    {
        UpdateShortestPaths(path, length);
    }

    for(auto str : *start)
    {
        int strLength = str.second;
        Crossing* next = str.first;
        if(!visited[next->getId()] && !contains(closed, next))
        {
            FindShortestPaths(next, end, visited, length + strLength, path, closed);
        }
    }

    path.pop_back();
    visited[start->getId()] = false;
}

void Map::UpdateShortestPaths(std::vector<Crossing*>& path, int length)
{
    int ind = shortestPathsCount;
    for(int i = 0; i < shortestPathsCount; i++)
    {
        if(shortestLengths[i] > length)
        {
            for(int j = 2; j >= i+1; j--)
            {
                shortestPaths[j] = shortestPaths[j-1];
                shortestLengths[j] = shortestLengths[j-1];
            }
            ind = i;
            return;
        }
    }
    shortestLengths[ind] = length;
    shortestPaths[ind] = path;
    if(shortestPathsCount < 2) shortestPathsCount++;
}

bool Map::HasCircuit(Crossing* start)
{
    bool *visited = new bool[crossingsCount];
    for(int i = 0; i < crossingsCount; i++)
    {
        visited[i] = false;
    }
    std::list<Crossing*> queue;
    visited[start->getId()] = true;
    queue.push_back(start);

    int steps = 0;

    while(!queue.empty())
    {
        Crossing* current = queue.front();
        queue.pop_front();
        
        std::map<Crossing*, int>::const_iterator it;
        for(it = current->begin(); it != current->end(); it++)
        {
            if((*it).first->getId() == start->getId() && steps > 0)
            {
                delete[] visited;
                return true;
            }
            if(!visited[(*it).first->getId()])
            {
                visited[(*it).first->getId()] = true;
                queue.push_back((*it).first);
                steps++;
            }
        }
    }
    delete[] visited;
    return false;
}

std::vector<Crossing*> Map::eulerianCircuit()
{
    Map map(*this);

    std::vector<Crossing*> circuit;
    if(!map.hasEulerianCircuit())
    {
        return circuit;
    }

    int* edgesCount = new int[map.crossingsCount];
    for(auto crossing:map.crossings)
    {
        edgesCount[crossing->getId()] = crossing->streetsCount();
    }

    std::stack<Crossing*> currentPath;

    Crossing* current = map.crossings[0];
    currentPath.push(current);

    while(!currentPath.empty())
    {
        if(edgesCount[current->getId()] > 0)
        {
            currentPath.push(current);
            std::pair<Crossing*, int> nextStr = *(current->begin());
            Crossing* next = nextStr.first;
            edgesCount[current->getId()]--;
            current->deleteStreet(next);
            current = next;
        }
        else
        {
            circuit.push_back(current);
            current = currentPath.top();
            currentPath.pop();
        }
    }
    std::reverse(circuit.begin(), circuit.end());
    return circuit;
} 

bool Map::hasEulerianCircuit()
{
    if(!isConnected())return false;

    int* indegrees = getIndegrees();

    for(auto crossing:crossings)
    {
        if(crossing->streetsCount() != indegrees[crossing->getId()])
        {
            delete[] indegrees;
            return false;
        }
    }

    delete[] indegrees;
    return true;
}

int* Map::getIndegrees()
{
    int* indegrees = new int[crossingsCount];
    for(int i = 0; i < crossingsCount; i++)
    {
        indegrees[i] = 0;
    }

    for(auto crossing:crossings)
    {
        int counter = 0;
        for(auto street:*crossing)
        {
            counter++;
            Crossing* next = street.first;
            indegrees[next->getId()]++;
        }
    }

    return indegrees;
}

bool Map::allReachableFrom(Crossing* start)
{
    bool* visited = new bool[crossingsCount];
    memset(visited, false, crossingsCount);
    std::list<Crossing*> queue;
    visited[start->getId()] = true;
    queue.push_back(start);

    while(!queue.empty())
    {
        Crossing* current = queue.front();
        queue.pop_front();
        for(auto street : *current)
        {
            Crossing* next = street.first;
            if(!visited[next->getId()])
            {
                visited[next->getId()] = true;
                queue.push_back(next);
            }
        }
    }

    for(int i = 0; i < crossingsCount; i++)
    {
        if(visited[i] == false)
        {
            delete[] visited;
            return false;
        }
    }

    delete[] visited;
    return true;
}

bool Map::allReachableFrom(int id)
{
    Crossing* crossing = findById(crossings, id);
    assert(crossing != nullptr);
    return allReachableFrom(crossing);
}

std::vector<DEStreet*> Map::deadEndStreets()
{
    std::vector<DEStreet*> deadEndStr;
    int* indegrees = getIndegrees();
    for(auto crossing : crossings)
    {
        if(indegrees[crossing->getId()] > 0)continue;
        std::vector<Crossing*> ends = deadEndStreetsFrom(crossing);
        for(auto end : ends)
        {
            DEStreet* str = new DEStreet(crossing->getName(), end->getName());
            deadEndStr.push_back(str);
        }
    }

    delete[] indegrees;
    return deadEndStr;
}

std::vector<Crossing*> Map::deadEndStreetsFrom(Crossing* start)
{
    std::vector<Crossing*> ends;
    bool* visited = new bool[crossingsCount];
    memset(visited, false, crossingsCount);

    visited[start->getId()] = true;
    std::list<Crossing*> queue;
    queue.push_back(start);
    while(!queue.empty())
    {
        Crossing* current = queue.front();
        queue.pop_front();
        if(current->streetsCount() == 0 && current->getId() != start->getId())
        {
            ends.push_back(current);
        }
        for(auto street : *current)
        {
            Crossing* next = street.first;
            if(!visited[next->getId()])
            {
                visited[next->getId()] = true;
                queue.push_back(next);
            }
        }
    }

    delete[] visited;
    return ends;
}


#endif