#include "User.h"

string roles[2] = { "User", "Admin" };

User::User()
{

}

User::User(string userName, string password)
{
	this->userName = userName;
	this->password = password;
}

bool RoleIsValid(string role)
{
	for (int i = 0; i < 2; i++)
	{
		if (role == roles[i])
		{
			return 1;
		}
	}

	return 0;
}

User::User(int id, string userName, string password, string role)
{
	this->id = id;
	this->userName = userName;
	this->password = password;
	if (RoleIsValid(role))
	{
		this->role = role;
	}
}

bool User::IsAdmin()
{
	return role == "Admin";
}

string User::ToString()
{
	string result =
		to_string(id) + '/'
		+ userName + '/'
		+ password + '/'
		+ role 
		+ '\n';

	return result;
}




User::~User()
{
}
