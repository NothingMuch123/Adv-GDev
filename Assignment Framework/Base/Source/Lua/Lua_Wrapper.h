#ifndef _LUA_WRAPPER_H_
#define _LUA_WRAPPER_H_

// lua
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <string>

using std::string;

class CLua_Wrapper
{
public:
	CLua_Wrapper();
	~CLua_Wrapper();

	// Opening and closing
	bool OpenLua(string filepath); // All scripts are in the folder "Lua_Scripts"
	bool CloseLua();

	string* GetString(string name);
	double* GetNumber(string name);
	bool* GetBool(string name);

	lua_State* GetState();

private:
	static const int S_TOP_OF_STACK;

	lua_State* m_luaState;
};

#endif