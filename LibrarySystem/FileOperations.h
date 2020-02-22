#pragma once
#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include "Book.h"
#include "User.h"
#include"Utils.h"

using namespace std;

class FileOperations
{
public:
	FileOperations();
	~FileOperations();

	static int ReadBooks(string file, vector<Book*>* destination);
	static int ReadUsers(string file, vector<User*>* destination);

	static string ReadFile(string file);

	static int WriteBooks(string file, vector<Book*> books);
	static int WriteUsers(string file, vector<User*> users);
};

