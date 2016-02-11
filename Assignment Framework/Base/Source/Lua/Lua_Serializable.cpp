#include "Lua_Serializable.h"

CLua_Serializable::CLua_Serializable()
{
}


CLua_Serializable::~CLua_Serializable()
{
}

void CLua_Serializable::write(fstream* file, string propertyName, int id, string value)
{
	(*file) << (propertyName + "_" + to_string((long long)id) + " = " + value + "\n").c_str();
}

double CLua_Serializable::read(CLua_Wrapper * lua, string propertyName, int id)
{
	double* data = nullptr;
	if (data = lua->GetNumber(propertyName + "_" + to_string((long long)id)))
	{
		return *data;
	}
	return 0.0;
}

/*void CLua_Serializable::read(CLua_Wrapper * lua, string propertyName, int id, string & value)
{
	string* data = nullptr;
	if (data = lua->GetString(propertyName + "_" + to_string((long long)id)))
	{
		value = *data;
	}
	else
	{
		value = "";
	}
}*/
