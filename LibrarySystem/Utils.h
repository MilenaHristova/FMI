#pragma once
#include<string>
#include<vector>
#include "Book.h"
using namespace std;

class Utils
{
public:
	Utils();
	~Utils();

	static vector<string> SplitString(string str, string delimiter);

	static string Join(vector<string> arr, char delimiter);

	static string ToLower(string str);

	static string InputPassword();
};

