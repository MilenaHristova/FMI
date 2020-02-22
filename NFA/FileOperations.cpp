#pragma once
#include "FileOperations.h"
#include "NFA.h"
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

FileOperations::FileOperations()
{
	this->filename = "";
}

bool FileOperations::FileIsOpen()
{
	return this->filename != "";
}

bool FileOperations::FileIsEmpty(std::fstream& file)
{
	return file.peek() == fstream::traits_type::eof();
}

void FileOperations::CreateEmptyFile(string filename)
{
	fstream fs;
	fs.open(filename, ios::out);
	this->filename = filename;
	fs.close();
}

void EnterStates(fstream& fs, map<int, State*>& statesMap, set<State*>& states, int count)
{
	for (int i = 0; i < count; i++)
	{
		int state;
		fs >> state;
		State* st = new State(state);
		statesMap.insert(pair<int, State*>(state, st));
		states.insert(st);
	}
}

void EnterFinalStates(fstream& fs, vector<int>& finals, map<int, State*>& statesMap, int count)
{
	for (int i = 0; i < count; i++)
	{
		int fin;
		fs >> fin;
		try
		{
			State* state = statesMap.at(fin);
		}
		catch (out_of_range exception)
		{
			throw("Final states must be listed with other states!");
		}

		finals.push_back(fin);
		statesMap.at(fin)->IsFinal = true;
	}
}

void EnterTransitions(fstream& fs, map<int, State*>& statesMap)
{
	int countTransitions;
	fs >> countTransitions;

	for (int i = 0; i < countTransitions; i++)
	{
		int current, countReachable, next;
		char symbol;
		fs >> current >> countReachable;

		State* state;
		try
		{
			state = statesMap.at(current);
		}
		catch (out_of_range exc)
		{
			throw("Invalid state on " + to_string(i) + "transition");
		}

		for (int j = 0; j < countReachable; j++)
		{
			fs >> symbol >> next;
			bool symbolIsValid = (symbol >= 'a' && symbol <= 'z') 
				|| symbol == 'E'
				|| symbol >= '0' && symbol <= '9';
			if (!symbolIsValid)
			{
				throw("Invalid symbol on " + to_string(i) + "transition");
			}
			try
			{

				State* nextState = statesMap.at(next);
				Transition* trans = new Transition(nextState, symbol);
				state->AddTransition(trans);
			}
			catch (out_of_range exc)
			{
				throw("Invalid state on " + to_string(i) + "transition");
			}
		}
	}
}

State* EnterInitialState(fstream& fs, map<int, State*>& statesMap)
{
	State* initial;
	int initialState;
	fs >> initialState;

	try
	{
		initial = statesMap.at(initialState);
	}
	catch (out_of_range exception)
	{
		throw("Initial state not found");
	}

	return initial;
}

NFA* FileOperations::ReadFile(string filename)
{
	int statesCount, finalsCount;
	map<int, State*> statesMap;
	set<State*> states;
	State* initial;
	vector<int> finalStates;
	
	fs.open(filename, ios::in);

	if (!fs.is_open())
	{
		CreateEmptyFile(filename);
		return nullptr;
	}

	if (FileIsEmpty(fs))
	{
		this->filename = filename;
		return nullptr;
	}

	try
	{
		fs >> statesCount;
		EnterStates(fs, statesMap, states, statesCount);
		initial = EnterInitialState(fs, statesMap);
		fs >> finalsCount;
		EnterFinalStates(fs, finalStates, statesMap, finalsCount);
		EnterTransitions(fs, statesMap);
	}
	catch (const char* msg)
	{
		throw msg;
	}
	
	NFA* nfa = new NFA(initial, statesCount, states, finalsCount, finalStates);
    
    this->filename = filename;
	fs.close();
    return nfa;
}

void FileOperations::Close()
{
	if (!FileIsOpen())
	{
		throw("First open a file.");
	}
	this->filename = "";
}

void WriteStates(fstream& fs, NFA* nfa)
{
	fs << nfa->GetCountOfStates() << " ";
	set<State*>::iterator it = nfa->statesBegin();
	for (it; it != nfa->statesEnd(); it++)
	{
		State* state = *it;
		fs << state->GetState() << " ";
	}
	fs << "\n";
}

void WriteFinalStates(fstream& fs, NFA* nfa)
{
	fs << nfa->GetCountOfFinalStates() << " ";
	vector<int>::iterator finalsIt;
	for (finalsIt = nfa->finalsBegin(); finalsIt != nfa->finalsEnd(); finalsIt++)
	{
		int fin = *finalsIt;
		fs << fin << " ";
	}
	fs << "\n";
}

void WriteTransitions(fstream& fs, NFA* nfa)
{
	fs << nfa->GetTransitionsCount() << "\n";
	set<State*>::iterator it;
	for (it = nfa->statesBegin(); it != nfa->statesEnd(); it++)
	{
		State* state = *it;
		if (state->TransitionsCount() > 0)
		{
			fs << state->GetState() << " " << state->TransitionsCount() << " ";
			set<Transition*>::iterator it;
			for (it = state->TransitionsBegin(); it != state->TransitionsEnd(); it++)
			{
				Transition* trans = *it;
				fs << (char)trans->GetSymbol() << " " << trans->GetNext()->GetState() << " ";
			}
			fs << "\n";
		}
	}
}

void WriteInitialState(fstream& fs, NFA* nfa)
{
	fs << nfa->GetInitialState()->GetState() << "\n";
}

void FileOperations::Save(NFA* nfa)
{
	if (!FileIsOpen())
	{
		throw("You can't save in the same file if no file is open.");
	}

	fs.open(filename, ios::out);

	WriteStates(fs, nfa);
	WriteInitialState(fs, nfa);
	WriteFinalStates(fs, nfa);
	WriteTransitions(fs, nfa);
	
	fs.close();
}

void FileOperations::SaveAs(NFA* nfa, string filename)
{
	this->filename = filename;
	Save(nfa);
}
