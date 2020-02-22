#include<vector>
#include<string>
#include "User.h"
#include "Book.h"

using namespace std;

class Library
{
public:
	vector<User*> Users;
	vector<Book*> Books;

	int lastUserId;
	int lastBookId;

	Library();
	Library(vector<User*> users, vector<Book*> books);
	~Library();

	void Initialize(string booksFileName, string usersFileName);
	void SaveData(string booksFileName, string usersFileName);

	User* FindUserByUsername(string username);
	Book* FindBookById(int id);

	vector<Book*> Search(string option, string searchTerm);
	vector<Book*> SearchBooksByTitle(string title);
	vector<Book*> SearchBooksByAuthor(string author);
	vector<Book*> SearchBooksByTag(string tag);

	vector<Book*> SortBooks(vector<Book*> books, int start, int end, string option);

	string AllBooksInfo();

	void AddBook(string title, string author, string genre, string summary,
		int year, double rating, vector<string>* keyWords);

	bool RemoveBook(string title, string author);
	
	void AddUser(string username, string password);
	bool RemoveUser(string username);

	bool UsernameIsAlreadyUsed(string username);

};

