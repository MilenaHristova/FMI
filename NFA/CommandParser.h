#pragma once
#include "FileOperations.h"
#include "NFA.h"
#include <vector>

class CommandParser
{
private:
	int automataRead;
	FileOperations fileOp;
	std::vector<NFA*> NFAs;
	NFA* Automat;
    
public:
	CommandParser();
	~CommandParser();
	void ReadCommand();
	void Open();
	void Close();
	void List();
	void Print(int id);
	void Save(int id, std::string filename);
	void Empty(int id);
	void Deterministic(int id);
	void Recognize();
	void Union();
	void Concat();
	void Un();
	void Regex();
};

