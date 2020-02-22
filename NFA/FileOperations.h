#pragma once
#include <string>
#include <fstream>
#include "NFA.h"

class FileOperations
{
	std::string filename;
	std::fstream fs;
public:
	FileOperations();
	bool FileIsOpen();
	NFA* ReadFile(std::string filename);
	void Close();
	void Save(NFA* nfa);
	void SaveAs(NFA* nfa, std::string filename);
	bool FileIsEmpty(std::fstream& file);
	void CreateEmptyFile(std::string filename);
};