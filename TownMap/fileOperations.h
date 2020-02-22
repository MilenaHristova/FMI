#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include<string>
#include<fstream>
#include<vector>
#include "crossing.h"

class FileOperations
{
    public:
    std::vector<Crossing*> read(std::string filename);
    void write(std::vector<Crossing*> map, std::string filename);
    std::vector<std::string> splitString(std::string str, std::string delimiter);
    Crossing* find(std::vector<Crossing*>& allCrossings, std::string name);
};

#endif