#pragma once

#include "lua/lua.hpp"

#ifdef _MSC_VER
#ifdef CENGINE_LIB_EXPORT
#define CENGINE_LIB_API __declspec( dllexport )
#else
#define CENGINE_LIB_API __declspec( dllimport )
#endif
#else
#define CENGINE_LIB_API
#endif

#define LUA_LOADLIB_COMMON "common"
#define LUA_LOADLIB_OBJECT "object"

extern "C"
{
    CENGINE_LIB_API void luaopen_cengine(lua_State *L);

    CENGINE_LIB_API int luaopen_common(lua_State *L);
    CENGINE_LIB_API int luaopen_object(lua_State *L);
}
