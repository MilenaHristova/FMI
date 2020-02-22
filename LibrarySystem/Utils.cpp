#include "Utils.h"
#include<string>
#include<vector>
#include<iostream>
#include<conio.h>
using namespace std;


Utils::Utils()
{
}


Utils::~Utils()
{
}

vector<string> Utils::SplitString(string str, string delimiter)
{
	vector<string> result;
	size_t position = str.find(delimiter);
	while (position != string::npos)
	{
		string token = str.substr(0, position);
		result.push_back(token);
		str.erase(0, position + delimiter.length());
		position = str.find(delimiter);
	}

	result.push_back(str);

	return result;
}

string Utils::Join(vector<string> vect, char delimiter)
{
	string result = "";

	vector<string>::iterator iterator;

	for (iterator = vect.begin(); iterator < vect.end(); iterator++)
	{
		result += (*iterator + delimiter);
	}

	return result;
}

string Utils::ToLower(string str)
{
	string result;
	for (int i = 0; i < str.length(); i++)
	{
		result += tolower(str[i]);
	}
	return result;
}

string Utils::InputPassword()
{
	string password;
	char p;

	p = _getch();
	while (p != 13)
	{
		if (p == '\b')
		{
			cout << "\b \b";
			password.pop_back();
		}
		else 
		{
			password.push_back(p);
			cout << '*';
		}
		
		p = _getch();
	}
	cout << endl;
	return password;
}