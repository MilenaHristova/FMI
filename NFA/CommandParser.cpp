#include "CommandParser.h"
#include "NFAConstruction.h"
#include <iostream>

using namespace std;

CommandParser::CommandParser()
{
	this->automataRead = 0;
	Automat = nullptr;
}

CommandParser::~CommandParser()
{
	std::vector<NFA*>::iterator it;
	for (it = NFAs.begin(); it != NFAs.end(); it++)
	{
		delete *it;
	}
}

void CommandParser::Open()
{
	std::string filename;
	cin >> filename;
	try
	{
		this->Automat = fileOp.ReadFile(filename);
	}
	catch (const char* msg)
	{
		cout << msg;
	}
	
	this->Automat->SetId(this->automataRead);
	this->NFAs.push_back(Automat);
	this->automataRead++;
	cout << "Successfuly opened file " << filename << endl;
}

void CommandParser::Close()
{
	try
	{
		fileOp.Close();
	}
	catch (const char* msg)
	{
		cout << msg;
	}
	Automat = new NFA();
	cout << "Seccessfuly closed file."<<endl;
}

void CommandParser::List()
{
	for (auto nfa : this->NFAs)
	{
		cout << nfa->GetId() << endl;
	}
}

void CommandParser::Print(int id)
{
	if (id >= this->NFAs.size())
	{
		cout << "Invalid Id." << endl;
		return;
	}
	cout << NFAs[id]->TransitionsInfo();
}

void CommandParser::Save(int id, string filename)
{
	if (id >= this->NFAs.size())
	{
		cout << "Invalid Id." << endl;
		return;
	}
	try
	{
		fileOp.SaveAs(NFAs[id], filename);
	}
	catch (const char* msg)
	{
		cout << msg;
	}
}

void CommandParser::Empty(int id)
{
	if (id >= this->NFAs.size())
	{
		cout << "Invalid Id." << endl;
		return;
	}
	NFA* nfa = NFAs[id];
	if (nfa->LanguageIsEmpty())
	{
		cout << "The language is empty."<<endl;
	}
	else cout << "The language is not empty."<<endl;
}

void CommandParser::Deterministic(int id)
{
	if (id >= this->NFAs.size())
	{
		cout << "Invalid Id." << endl;
		return;
	}
	NFA* nfa = NFAs[id];
	if (nfa->IsDeterministic())
	{
		cout << "Deterministic"<<endl;
	}
	else cout << "Nondeterministic"<<endl;
}

void CommandParser::Recognize()
{
	int id;
	string word;
	cin >> id >> word;
	if (id >= this->NFAs.size())
	{
		cout << "Invalid Id." << endl;
		return;
	}
	NFA* nfa = NFAs[id];
	cout << nfa->Accept(word)<<endl;
}

void CommandParser::Union()
{
	NFAConstruction constr;
	int id1, id2;
	cin >> id1 >> id2;
	if (id1 >= this->NFAs.size() || id2 > this->NFAs.size())
	{
		cout << "Invalid NFA IDs."<<endl;
		return;
	}

	NFA* first = NFAs[id1];
	NFA* second = NFAs[id2];
	NFA* nfa = constr.Union(first, second);
	nfa->SetId(this->automataRead);
	cout << this->automataRead;
	this->automataRead++;
	this->NFAs.push_back(nfa);
}

void CommandParser::Concat()
{
	NFAConstruction constr;
	int id1, id2;
	cin >> id1 >> id2;
	if (id1 >= this->NFAs.size() || id2 > this->NFAs.size())
	{
		cout << "Invalid NFA IDs."<<endl;
		return;
	}

	NFA* first = NFAs[id1];
	NFA* second = NFAs[id2];
	NFA* nfa = constr.Concat(first, second);
	nfa->SetId(this->automataRead);
	cout << this->automataRead;
	this->automataRead++;
	this->NFAs.push_back(nfa);
}

void CommandParser::Un()
{
	NFAConstruction constr;
	int id1;
	cin >> id1;
	if (id1 >= this->NFAs.size())
	{
		cout << "Invalid NFA ID."<<endl;
		return;
	}

	NFA* first = NFAs[id1];
	NFA* nfa = constr.Un(first);
	nfa->SetId(this->automataRead);
	cout << this->automataRead;
	this->automataRead++;
	this->NFAs.push_back(nfa);
}

void CommandParser::Regex()
{
	NFAConstruction constr;
	string regex;
	cin >> regex;
	NFA* nfa = constr.FromRegEx(regex);
	nfa->SetId(this->automataRead);
	cout << this->automataRead;
	this->automataRead++;
	this->NFAs.push_back(nfa);
}

void CommandParser::ReadCommand()
{
	std::string command;
	while (true)
	{
		cin >> command;
		if (command == "open")
		{
			Open();
		}
		else if (command == "close")
		{
			Close();
		}
		else if (command == "print")
		{
			int id;
			cin >> id;
			Print(id);
		}
		else if (command == "save")
		{
			int id;
			string filename;
			cin >> id >> filename;
			Save(id, filename);
		}
		else if (command == "saveas")
		{
			std::string filename;
			cin >> filename;
			fileOp.SaveAs(Automat, filename);
		}
		else if (command == "exit")
		{
			cout << "Exiting the program.";
			exit(0);
		}
		else if (command == "list")
		{
			List();
		}
		else if (command == "empty")
		{
			int id;
			cin >> id;
			Empty(id);
		}
		else if (command == "deterministic")
		{
			int id;
			cin >> id;
			Deterministic(id);
		}
		else if (command == "recognize")
		{
			Recognize();
		}
		else if (command == "union")
		{
			Union();
		}
		else if (command == "concat")
		{
			Concat();
		}
		else if (command == "un")
		{
			Un();
		}
		else if (command == "reg")
		{
			Regex();
		}
	}
}