#include "cengine.hpp"

static const luaL_Reg loadedlibs[] = {
    { LUA_LOADLIB_COMMON, luaopen_common },
    { LUA_LOADLIB_OBJECT, luaopen_object },
    { NULL, NULL },
};

void luaopen_cengine(lua_State *L)
{
    const luaL_Reg *lib;
    for (lib = loadedlibs; lib->func; lib++) {
        luaL_requiref(L, lib->name, lib->func, 1);
        lua_pop(L, 1);
    }
}
