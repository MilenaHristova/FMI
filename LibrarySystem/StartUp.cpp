#include<iostream>
#include<fstream>
#include<vector>
#include<conio.h>
#include "User.h"
#include "Book.h"
#include "Library.h"
#include "Utils.h"
#include "FileOperations.h"

using namespace std;

User* loggedInUser;
Library* library;
const string booksFileName = "books.txt";
const string usersFileName = "users.txt";
const string helpFileName = "help.txt";


bool IsLoggedIn()
{
	if (loggedInUser == NULL)
	{
		cout << "You have to log in first.";
		return 0;
	}
	return 1;
}

bool IsAdmin()
{
	if (loggedInUser == NULL || !loggedInUser->IsAdmin())
	{
		cout << "You have to be admin to do this.";
		return 0;
	}

	return 1;
}

void Login()
{
	if (loggedInUser != NULL)
	{
		cout << "You are already logged in." << endl;
		return;
	}

	string username, password;
	char p;
	cout << "Enter username: ";
	cin >> username;

	cout << "Enter password: ";
	password = Utils::InputPassword();

	User* user = library->FindUserByUsername(username);
	if (user == NULL || user->password != password)
	{
		cout << "Incorrect username or password.";
		return;
	}

	loggedInUser = user;
	cout << "Welcome, " << username << endl;
}

void Logout()
{
	if (IsLoggedIn())
	{
		loggedInUser = NULL;
		cout << "Logged out.";
	}
}

void Help()
{
	string help = FileOperations::ReadFile(helpFileName);
	cout << help;
}

void Exit()
{
	char answerExit, answerSave;

	cout << "Are you sure you want to exit the program?(Y/N): ";
	cin >> answerExit;

	if (toupper(answerExit) == 'Y')
	{
		cout << "Do you want to save the changes on the data?(Y/N): ";
		cin >> answerSave;
		if (toupper(answerSave) == 'Y')
		{
			library->SaveData(booksFileName, usersFileName);
		}

		exit(0);
	}
}

void BooksAll()
{
	if (IsLoggedIn())
	{
		cout << library->AllBooksInfo();
	}
}

void BooksInfo(int id)
{
	if (!IsLoggedIn()) return;

	Book* book = library->FindBookById(id);

	if (book == NULL)
	{
		cout << "Book not found.";
		return;
	}

	cout << book->FullInfo();
}

void FindBook(string option, string searchTerm)
{
	if (!IsLoggedIn()) return;

	string optionToLower = Utils::ToLower(option);
	string searchTermToLower = Utils::ToLower(searchTerm);

	vector<Book*>::iterator iterator;
	vector<Book*> books = library->Search(optionToLower, searchTermToLower);

	for (iterator = books.begin(); iterator < books.end(); iterator++)
	{
		Book* b = *iterator;
		cout << b->ShortInfo();
	}
}

void Sort(string option)
{
	if (!IsLoggedIn()) return;

	vector<Book*> books(library->Books);
	vector<Book*> sortedBooks = library->SortBooks(books, 0, books.size() - 1, option);

	vector<Book*>::iterator iterator;
	for (iterator = books.begin(); iterator < books.end(); iterator++)
	{
		cout << (*iterator)->ShortInfo() << endl;
	}
}

void AddBook()
{
	if (!IsAdmin()) return;

	string title, author, genre, summary, keyWordsStr;
	vector<string> keyWords;
	string keyWord;
	int year;
	double rating;

	cout << "Title: ";
	getline(cin, title);
	cout << "Author: ";
	getline(cin, author);
	cout << "Genre: ";
	getline(cin, genre);
	cout << "Summary: ";
	getline(cin, summary);
	cout << "Year of publishing: ";
	cin >> year;
	cout << "Rating: ";
	cin >> rating;
	cout << "Key words(','-separated): " << endl;
	cin.ignore();
	getline(cin, keyWordsStr);

	keyWords = Utils::SplitString(keyWordsStr, ",");

	library->AddBook(title, author, genre, summary,
		year, rating, &keyWords);

	cout << "Successfuly added book.";
}

void RemoveBook()
{
	if (!IsAdmin()) return;

	string title, author;
	char answer;

	cout << "Enter book title: ";
	getline(cin, title);
	cout << "Enter book author: ";
	getline(cin, author);
	cout << "Are you sure you want to delete " + title + " by " + author + "(Y/N: )";
	cin >> answer;
	if (toupper(answer) == 'Y')
	{
		bool success = library->RemoveBook(title, author);
		if (success)
		{
			cout << "Successfuly deleted.";
		}
		else cout << "Book not found!";
	}
}

void AddUser()
{
	if (!IsAdmin()) return;

	string username, password, repeatPassword;
	cout << "Username: ";
	cin >> username;

	while (library->UsernameIsAlreadyUsed(username))
	{
		cout << "Username is already used!";
		cout << "Username: ";
		cin >> username;
	}

	cout << "Password: ";
	password = Utils::InputPassword();

	cout << "Repeat password: ";
	repeatPassword = Utils::InputPassword();

	while (password != repeatPassword)
	{
		cout << "Passwords do not match!";
		cout << "Password: ";
		password = Utils::InputPassword();
		cout << "Repeat password: ";
		repeatPassword = Utils::InputPassword();
	}

	library->AddUser(username, password);
	cout << "Successful registration.";
}

void RemoveUser()
{
	if (!IsAdmin()) return;

	string username;
	char answer;

	cout << "Enter username: ";
	cin >> username;
	cout << "Are you sure you want to delete " + username + "(Y/N: )";
	cin >> answer;
	if (toupper(answer) == 'Y')
	{
		bool success = library->RemoveUser(username);
		if (success)
		{
			cout << "Successfuly deleted user.";
		}
		else cout << "User not found!";
	}
}

bool CommandIsValid(vector<string> tokens)
{
	if (tokens[0] == "books")
	{
		if (tokens[1] == "info" && tokens.size() != 3)return false;
		if ((tokens[1] == "find" || tokens[1] == "sort")
			&& tokens.size() != 4) return false;
		if ((tokens[1] == "all"
			|| tokens[1] == "add"
			|| tokens[1] == "remove")
			&& tokens.size() != 2) return false;
	}
	if (tokens[0] == "users")
	{
		if ((tokens[1] == "add"
			|| tokens[1] == "remove")
			&& tokens.size() != 2) return false;
	}

	return true;
}

void ParseCommand()
{
	while (true)
	{
		string command;
		getline(cin, command);
		if (command == "exit")
		{
			Exit();
		}
		else if (command == "login")
		{
			Login();
		}
		else if (command == "logout")
		{
			Logout();
		}
		else if (command == "help")
		{
			Help();
		}
		else
		{
			vector<string> tokens = Utils::SplitString(command, " ");

			if (!CommandIsValid)
			{
				cout << "Invalid command" << endl;
				continue;
			}

			if (tokens[0] == "books")
			{
				if (tokens[1] == "all")
				{
					BooksAll();
				}
				if (tokens[1] == "info")
				{
					BooksInfo(stoi(tokens[2]));
				}
				else if (tokens[1] == "find")
				{
					FindBook(tokens[2], tokens[3]);
				}
				else if (tokens[1] == "sort")
				{
					Sort(tokens[2]);
				}
				else if (tokens[1] == "add")
				{
					AddBook();
				}
				else if (tokens[1] == "remove")
				{
					RemoveBook();
				}
			}
			else if (tokens[0] == "users")
			{
				if (tokens[1] == "add")
				{
					AddUser();
				}
				else if (tokens[1] == "remove")
				{
					RemoveUser();
				}
			}
			else continue;
		}
	}
}



int main()
{
	library = new Library();

	library->Initialize(booksFileName, usersFileName);

	ParseCommand();
}
