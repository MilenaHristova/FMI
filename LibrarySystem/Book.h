#pragma once
#include<string>
#include<vector>
#include"Utils.h"

using namespace std;

class Book
{
public:
	Book();
	Book(int id,
		string author, 
		string title, 
		string genre,
		string summary,
		int year,
		double rating,
		vector<string>* keyWords);

	string ShortInfo();
	string FullInfo();

	string ToString();
	~Book();

	string author;
	string title;
	string genre;
	string summary;
	int yearOfPublishing;
	vector<string> keyWords;
	double rating;
	int id;	
};

