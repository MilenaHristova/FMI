#pragma once
#include<string>

using namespace std;

class User
{
public:
	int id;
	string userName;
	string password;
	string role;
	User();
	User(string userName, string password);
	User(int id, string userName, string password, string role);
	~User();
	bool IsAdmin();
	string ToString();
};

