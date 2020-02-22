#include "Library.h"
#include "Utils.h"
#include "FileOperations.h"
#include<string>
#include<fstream>
#include<vector>

using namespace std;

Library::Library()
{
	
}

Library::Library(std::vector<User*> users, std::vector<Book*> books)
{
	Users = users;
	Books = books;
}

Library::~Library()
{
	vector<Book*>::iterator booksIterator;

	for (booksIterator = Books.begin(); booksIterator < Books.end(); booksIterator++)
	{
		delete *booksIterator;
	}

	vector<User*>::iterator usersIterator;

	for (usersIterator = Users.begin(); usersIterator < Users.end(); usersIterator++)
	{
		delete *usersIterator;
	}

	delete &Books;
	delete &Users;
}

void Library::Initialize(string booksFileName, string usersFileName)
{
	int booksCount = FileOperations::ReadBooks(booksFileName, &Books);
	int usersCount = FileOperations::ReadUsers(usersFileName, &Users);

	if (booksCount == 0)
	{
		this->lastBookId = 0;
	}
	else
	{
		this->lastBookId = Books.back()->id;
	}
	if (usersCount == 0)
	{
		this->lastUserId = 0;
	}
	else this->lastUserId = Users.back()->id;

	if (Users.size() == 0)
	{
		User* admin = new User("admin", "i<3c++");
		admin->id = 1;
		admin->role = "Admin";
		Users.push_back(admin);

		this->lastUserId = 1;
	}
}

void Library::SaveData(string booksFileName, string usersFileName)
{
	int booksCount = FileOperations::WriteBooks(booksFileName, Books);
	int usersCount = FileOperations::WriteUsers(usersFileName, Users);

	if (booksCount == Books.size()
		&& usersCount == Users.size())
	{
		cout << "All data is saved.";
	}
	else cout << "There was a problem saving all the records.";
}
Book* Library::FindBookById(int id)
{
	vector<Book*>::iterator iterator;
	for (iterator = Books.begin(); iterator < Books.end(); iterator++)
	{
		if ((*iterator)->id == id)
		{
			return (*iterator);
		}
	}

	return NULL;
}

User* Library::FindUserByUsername(string username)
{
	vector<User*>::iterator iterator;
	for (iterator = Users.begin(); iterator < Users.end(); iterator++)
	{
		if ((*iterator)->userName == username)
		{
			return (*iterator);
		}
	}

	return NULL;
}

string Library::AllBooksInfo()
{
	string result = "";

	vector<Book*>::iterator iterator;
	for (iterator = Books.begin(); iterator < Books.end(); iterator++)
	{
		result += (*iterator)->ShortInfo();
		result += '\n';
	}

	return result;
}

vector<Book*> Library::SearchBooksByTitle(string title)
{
	vector<Book*> books;

	vector<Book*>::iterator iterator;

	for (iterator = Books.begin(); iterator < Books.end(); iterator++)
	{
		string bookTitle = Utils::ToLower((*iterator)->title);

		if (bookTitle.find(title) != string::npos)
		{
			books.push_back(*iterator);
		}
	}

	return books;
}

vector<Book*> Library::SearchBooksByAuthor(string author)
{
	vector<Book*> books;

	vector<Book*>::iterator iterator;

	for (iterator = Books.begin(); iterator < Books.end(); iterator++)
	{
		string booksAuthor = Utils::ToLower((*iterator)->author);

		if (booksAuthor == author)
		{
			books.push_back(*iterator);
		}
	}

	return books;
}

vector<Book*> Library::SearchBooksByTag(string tag)
{
	vector<Book*> books;

	vector<Book*>::iterator booksIterator;
	vector<string>::iterator tagsIterator;

	for (booksIterator = Books.begin(); booksIterator < Books.end(); booksIterator++)
	{
		Book* book = *booksIterator;
		vector<string> keyWords = book->keyWords;

		for (tagsIterator = keyWords.begin(); tagsIterator < keyWords.end(); tagsIterator++)
		{
			string currentTag = Utils::ToLower(*tagsIterator);

			if (currentTag == tag)
			{
				books.push_back(book);
				break;
			}
		}
	}

	return books;
}

vector<Book*> Library::Search(string option, string searchTerm)
{
	if (option == "title")
	{
		return SearchBooksByTitle(searchTerm);
	}
	else if (option == "author")
	{
		return SearchBooksByAuthor(searchTerm);
	}
	else
	{
		return SearchBooksByTag(searchTerm);
	}
}


void Swap(Book* first, Book* second)
{
	Book* temp = first;
	first = second;
	second = temp;
}

int Partition(vector<Book*> books, int start, int end, string option)
{
	double pivot;
	if (option == "id")
	{
		pivot = books[end]->id;
	}
	else if (option == "year")
	{
		pivot = books[end]->yearOfPublishing;
	}
	else pivot = books[end]->rating;

	int i = start - 1;

	for (int j = start; j < end; j++)
	{
		double valueToCompare;

		if (option == "id")
		{
			valueToCompare = books[end]->id;
		}
		else if (option == "year")
		{
			valueToCompare = books[end]->yearOfPublishing;
		}
		else valueToCompare = books[end]->rating;

		if (valueToCompare <= pivot)
		{
			i++;
			Swap(books[i], books[j]);
		}
	}

	Swap(books[i + 1], books[end]);
	return i + 1;
}

vector<Book*> Library::SortBooks(vector<Book*> books, int start, int end, string option)
{
	if (start < end)
	{
		int pi = Partition(books, start, end, option);

		SortBooks(books, pi, start - 1, option);
		SortBooks(books, pi + 1, end, option);
	}

	return books;
}


void Library::AddBook(string title, string author, string genre, string summary,
	int year, double rating, vector<string>* keyWords)
{
	int bookId = lastBookId + 1;
	Book* book = new Book(bookId, author, title, genre, summary, year, rating, keyWords);
	this->Books.push_back(book);
	lastBookId++;
}

void Library::AddUser(string username, string password)
{
	int userId = lastUserId + 1;
	User* user = new User(userId, username, password, "User");
	this->Users.push_back(user);
	lastUserId++;
}

bool Library::RemoveUser(string username)
{
	vector<User*>::iterator iterator;

	for (iterator = Users.begin(); iterator < Users.end(); iterator++)
	{
		if ((*iterator)->userName == username)
		{
			Users.erase(iterator);
			return true;
		}
	}

	return false;
}

bool Library::RemoveBook(string title, string author)
{
	vector<Book*>::iterator iterator;

	for (iterator = Books.begin(); iterator < Books.end(); iterator++)
	{
		if ((*iterator)->title == title && (*iterator)->author == author)
		{
			Books.erase(iterator);
			return true;
		}
	}

	return false;	
}

bool Library::UsernameIsAlreadyUsed(string username)
{
	vector<User*>::iterator iterator;
	for (iterator = Users.begin(); iterator < Users.end(); iterator++)
	{
		if ((*iterator)->userName == username)
		{
			return true;
		}
	}

	return false;
}
