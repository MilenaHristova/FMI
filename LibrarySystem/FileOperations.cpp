#include "FileOperations.h"



FileOperations::FileOperations()
{
}


FileOperations::~FileOperations()
{
}

string FileOperations::ReadFile(string file)
{
	ifstream in(file);
	string str((istreambuf_iterator<char>(in)),
		istreambuf_iterator<char>());

	return str;
}

int FileOperations::ReadBooks(string file, vector<Book*>* destination)
{
	ifstream in(file);
	if (!in)
	{
		cout << "Cannot open file.";
		return -1;
	}

	string str;
	int count = 0;

	while (getline(in, str))
	{
		if (str != "")
		{
			vector<string> tokens = Utils::SplitString(str, "/");
			int id = stoi(tokens[0]);
			string title = tokens[1];
			string author = tokens[2];
			string genre = tokens[3];
			string summary = tokens[4];
			int year = stoi(tokens[5]);
			double rating = stod(tokens[6]);

			vector<string> keyWords = Utils::SplitString(tokens[7], ",");
		
			Book* book = new Book(id, title, author, genre, summary, year, rating, &keyWords);
			
			(*destination).push_back(book);
			count++;
		}	
	}
	in.close();
	return count;
}

int FileOperations::ReadUsers(string file, vector<User*>* destination)
{
	ifstream in(file);
	if (!in)
	{
		cout << "Cannot open file.";
		return -1;
	}

	string str;
	int count = 0;

	while (getline(in, str))
	{
		if (str != "")
		{
			vector<string> tokens = Utils::SplitString(str, "/");
			int id = stoi(tokens[0]);
			string username = tokens[1];
			string password = tokens[2];
			string role = tokens[3];

			User* user = new User(id, username, password, role);

			(*destination).push_back(user);
			count++;
		}

		in.close();
		return count;
	}
}
		


int FileOperations::WriteBooks(string file, vector<Book*> books)
{
	ofstream fout(file);
	if (!fout)
	{
		return -1;
	}

	vector<Book*>::iterator iterator;
	int index = 0;

	for (iterator = books.begin(); iterator < books.end(); iterator++)
	{
		string result = (*iterator)->ToString();
			
		fout << result;

		index++;
	}

	fout.close();

	return index;
}

int FileOperations::WriteUsers(string file, vector<User*> users)
{
	ofstream fout(file);
	if (!fout)
	{
		return -1;
	}

	vector<User*>::iterator iterator;
	int index = 0;

	for (iterator = users.begin(); iterator < users.end(); iterator++)
	{
		string result = (*iterator)->ToString();

		fout << result;

		index++;
	}

	fout.close();

	return index;
}
