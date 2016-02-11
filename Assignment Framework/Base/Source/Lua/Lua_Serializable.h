#ifndef LUA_SERIALIZABLE_H
#define LUA_SERIALIZABLE_H

#include <string>
#include <fstream>
#include "Lua_Wrapper.h"

using std::string;
using std::to_string;
using std::fstream;

class CLua_Serializable
{
public:
	CLua_Serializable();
	~CLua_Serializable();

	virtual bool SaveState(fstream* file, int id) = 0;
	virtual bool LoadState(CLua_Wrapper* lua, int id) = 0;

protected:
	virtual void write(fstream* file, string propertyName, int id, string value);
	virtual double read(CLua_Wrapper* lua, string propertyName, int id);
	//virtual void read(CLua_Wrapper* lua, string propertyName, int id, string& value);
};

#endif