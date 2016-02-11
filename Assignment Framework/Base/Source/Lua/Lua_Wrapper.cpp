#include "Lua_Wrapper.h"

const int CLua_Wrapper::S_TOP_OF_STACK = -1;

CLua_Wrapper::CLua_Wrapper()
	: m_luaState(nullptr)
{
}


CLua_Wrapper::~CLua_Wrapper()
{
	if (m_luaState)
	{
		lua_close(m_luaState);
		m_luaState = nullptr;
	}
}

bool CLua_Wrapper::OpenLua(string filepath)
{
	if (!m_luaState) // If state does not exist
	{
		m_luaState = lua_open(); // Create a space for lua
		luaL_openlibs(m_luaState); // Open libs for usage
	}

	if (luaL_loadfile(m_luaState, filepath.c_str()) || lua_pcall(m_luaState, 0, 0, 0))
	{
		return false; // Fail to open
	}
	return true; // Opened successfully
}

bool CLua_Wrapper::CloseLua()
{
	if (m_luaState) // State exists for closing
	{
		lua_close(m_luaState);
		m_luaState = nullptr;
		return true; // Close successfully
	}
	return false; // Nothing to close
}

string * CLua_Wrapper::GetString(string name)
{
	string* data = new string(); // Create result

	lua_getglobal(m_luaState, name.c_str());
	if (!lua_isstring(m_luaState, S_TOP_OF_STACK))
	{
		// Incorrect data type
		return nullptr;
	}
	*data = lua_tostring(m_luaState, S_TOP_OF_STACK);
	lua_pop(m_luaState, S_TOP_OF_STACK);
	return data;
}

double * CLua_Wrapper::GetNumber(string name)
{
	double* data = new double();

	lua_getglobal(m_luaState, name.c_str());
	if (!lua_isnumber(m_luaState, S_TOP_OF_STACK))
	{
		// Incorrect data type
		return nullptr;
	}
	*data = lua_tonumber(m_luaState, S_TOP_OF_STACK);
	lua_pop(m_luaState, S_TOP_OF_STACK);
	return data;
}

bool * CLua_Wrapper::GetBool(string name)
{
	bool* data = new bool();

	lua_getglobal(m_luaState, name.c_str());
	if (!lua_isboolean(m_luaState, S_TOP_OF_STACK))
	{
		// Incorrect data type
		return nullptr;
	}
	*data = lua_toboolean(m_luaState, S_TOP_OF_STACK);
	lua_pop(m_luaState, S_TOP_OF_STACK);
	return data;
}
