#include "Book.h"
using namespace std;


Book::Book()
{

}

Book::Book(int id,
	string author,
	string title,
	string genre,
	string summary,
	int year,
	double rating,
	vector<string>* keyWords)
{
	this->author = author;
	this->title = title;
	this->id = id;
	this->summary = summary;
	this->genre = genre;
	this->keyWords = *keyWords;
	this->yearOfPublishing = year;
	this->rating = rating;
}


Book::~Book()
{
}

string Book::ShortInfo()
{
	string bookInfo =
		 "Title: " + title
		+ "\n" + "Author: " + author
		+ "\n" + "Genre: " + genre
		+ "\n" + "Id: " + to_string(id);

	return bookInfo;
}

string Book::FullInfo()
{
	string bookInfo =
		ShortInfo()
		+ "\n" + "Summary: " + summary
		+ "\n" + "Year of publishing: " + to_string(yearOfPublishing)
		+ "\n" + "Rating: " + to_string(rating)
		+ "\n" + "Key Words:";

	vector<string>::iterator tagIterator;

	for (tagIterator = keyWords.begin(); tagIterator < keyWords.end(); tagIterator++)
	{
		bookInfo += "#" + *tagIterator;
	}
		
	return bookInfo;
}

string Book::ToString()
{
	string result =
		to_string(id) + '/'
		+ title + '/'
		+ author + '/'
		+ genre + '/'
		+ summary + '/'
		+ to_string(yearOfPublishing) + '/'
		+ to_string(rating) + '/'
		+ Utils::Join(keyWords, ',')
		+ '\n';

	return result;
}

