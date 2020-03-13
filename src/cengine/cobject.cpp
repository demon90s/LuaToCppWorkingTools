#include "cengine.hpp"
#include <cstring>

#define OBJECT_METATABLE "OBJECT_METATABLE"

#define CHECK_USERDATA_OBJECT()\
    static_cast<Object*>(luaL_checkudata(L, 1, OBJECT_METATABLE));

struct Object
{
    Object() = default;
    int value = 0;
};

extern "C"
{
    int object_new(lua_State *L)
    {
        auto object = static_cast<Object*>(lua_newuserdata(L, sizeof(Object)));
        memset(object, 0, sizeof(Object));

        new (object) Object();

        luaL_getmetatable(L, OBJECT_METATABLE);
        lua_setmetatable(L, -2);

        return 1;
    }

    int object_free(lua_State *L)
    {
        Object *obj = CHECK_USERDATA_OBJECT();
        obj->~Object();

        return 0;
    }

    int object_setvalue(lua_State *L)
    {
        Object *obj = CHECK_USERDATA_OBJECT();
        luaL_checkinteger(L, 2);

        int value = static_cast<int>(lua_tointeger(L, 2));
        obj->value = value;

        return 0;
    }

    int object_getvalue(lua_State *L)
    {
        Object *obj = CHECK_USERDATA_OBJECT();
        lua_pushinteger(L, obj->value);

        return 1;
    }
}

static const struct luaL_Reg object_f[] = {
    { "New", object_new },
    { NULL, NULL },
};

static const struct luaL_Reg object_m[] = {
    { "Free", object_free },
    { "SetValue", object_setvalue },
    { "GetValue", object_getvalue },
    { NULL, NULL },
};

int luaopen_object(lua_State *L)
{
    luaL_newmetatable(L, OBJECT_METATABLE);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, object_m, 0);

    luaL_newlib(L, object_f);
    return 1;
}