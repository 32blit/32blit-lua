#include "../luablitlib.hpp"

using namespace blit;

static int vec2_new(lua_State* L){
    int nargs = lua_gettop(L);
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    lua_pop(L, nargs);
    new(lua_newuserdata(L, sizeof(Vec2))) Vec2(x, y);
    luaL_setmetatable(L, LUA_BLIT_VEC2);
    return 1;
}

static int vec2_delete(lua_State* L){
    reinterpret_cast<Vec2*>(lua_touserdata(L, 1))->~Vec2();
    lua_pop(L, 1);
    return 0;
}

static int vec2_index(lua_State* L){
    int nargs = lua_gettop(L);
    Vec2 *vec2 = reinterpret_cast<Vec2*>(lua_touserdata(L, 1));
    std::string method = luaL_checkstring(L, 2);
    if (nargs == 3) {
        float value = luaL_checknumber(L, 3);
        lua_pop(L, nargs);
        if(method == "x") {vec2->x = value; return 0;}
        if(method == "y") {vec2->y = value; return 0;}
    }
    lua_pop(L, nargs);
    if(method == "x") {lua_pushnumber(L, vec2->x); return 1;}
    if(method == "y") {lua_pushnumber(L, vec2->y); return 1;}

    luaL_error(L, "Unknown property or method `%s` on %s", method.c_str(), LUA_BLIT_VEC2);
    return 0;
}

Vec2* lua_blit_checkvec2(lua_State *L, int arg) {
    auto vec2 = luaL_checkudata(L, arg, LUA_BLIT_VEC2);
    return reinterpret_cast<Vec2*>(vec2);
}

void lua_blit_register_vec2(lua_State *L) {
    lua_register(L, LUA_BLIT_VEC2, vec2_new);
    luaL_newmetatable(L, LUA_BLIT_VEC2);
    lua_pushcfunction(L, vec2_delete); lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, vec2_index); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, vec2_index); lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
}