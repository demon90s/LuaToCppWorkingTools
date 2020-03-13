#include <iostream>
#include "lua/lua.hpp"
#include "cengine/cengine.hpp"

/*
** Create the 'arg' table, which stores all arguments from the
** command line ('argv'). It should be aligned so that, at index 0,
** it has 'argv[script]', which is the script name. The arguments
** to the script (everything after 'script') go to positive indices;
** other arguments (before the script name) go to negative indices.
** If there is no script name, assume interpreter's name as base.
*/
static void createargtable (lua_State *L, char **argv, int argc, int script) {
  int i, narg;
  if (script == argc) script = 0;  /* no script name? */
  narg = argc - (script + 1);  /* number of positive indices */
  lua_createtable(L, narg, script + 1);
  for (i = 0; i < argc; i++) {
    lua_pushstring(L, argv[i]);
    lua_rawseti(L, -2, i - script);
  }
  lua_setglobal(L, "arg");
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "usage: %s <lua_script_dir>\n", argv[0]);
		std::cin.get();
		return -1;
	}

	std::string lua_file = std::string(argv[1]) + "/main.lua";

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaopen_cengine(L);
	createargtable(L, argv, argc, 1);

	if (luaL_dofile(L, lua_file.c_str()) != LUA_OK)
	{
		fprintf(stderr, "[LUA-ERROR]: %s\n", lua_tostring(L, -1));
	}

	lua_close(L);

	std::cin.get();
}