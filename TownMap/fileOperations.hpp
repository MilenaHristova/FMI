#include "fileOperations.h"
#include<iostream>
#include<fstream>

Crossing* FileOperations::find(std::vector<Crossing*>& allCrossings, std::string name)
{
    std::vector<Crossing*>::iterator it;
    for(it = allCrossings.begin(); it != allCrossings.end(); it++)
    {
        if((*it)->getName() == name)
        {
            return *it;
        }
    }
    return nullptr;
}

std::vector<std::string> FileOperations::splitString(std::string str, std::string delimiter)
{
	std::vector<std::string> result;
	size_t position = str.find(delimiter);
	while (position != std::string::npos)
	{
		std::string token = str.substr(0, position);
		result.push_back(token);
		str.erase(0, position + delimiter.length());
		position = str.find(delimiter);
	}

	result.push_back(str);

	return result;
}

std::vector<Crossing*> FileOperations::read(std::string filename)
{
    std::vector<Crossing*> allCrossings;
	std::ifstream in(filename);

	std::string str;
    int crossId = 0;
    int streetId = 0;

	while (std::getline(in, str))
	{
		if (str != "")
		{
			std::vector<std::string> tokens = splitString(str, " ");
            std::string name = tokens[0];
            Crossing* cross = find(allCrossings, name);
            if(cross == nullptr)
            {
                cross = new Crossing(name, crossId);
                crossId++;
                allCrossings.push_back(cross);
            }
            
            std::vector<Street*> streets;
            std::vector<std::string>::iterator it = tokens.begin();
            it++;
            
            for(it; it != tokens.end(); it++)
            {
                std::string nextName = *it;
                it++;
                int length = std::stoi(*it);
                Crossing* next = find(allCrossings, nextName);
                if(next == nullptr)
                {
                    next = new Crossing(nextName, crossId);
                    crossId++;
                    allCrossings.push_back(next);
                }
                cross->addStreet(next, length);
            }
		}
	}
	in.close();
	return allCrossings;
}

void FileOperations::write(std::vector<Crossing*> map, std::string filename)
{
    std::fstream out;
    out.open(filename, std::ios::out);

    for(auto crossing : map)
    {
        out<<crossing->getName()<<" ";
        for(auto street : *crossing)
        {
            out<<street.first->getName()<<" "<<street.second<<" ";
        }
        out<<std::endl;
    }

    out.close();
}