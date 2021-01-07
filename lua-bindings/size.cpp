#include "../luablitlib.hpp"

using namespace blit;

static int size_new(lua_State* L){
    int nargs = lua_gettop(L);
    int32_t x = (int32_t)luaL_checknumber(L, 1);
    int32_t y = (int32_t)luaL_checknumber(L, 2);
    lua_pop(L, nargs);
    new(lua_newuserdata(L, sizeof(Size))) Size(x, y);
    luaL_setmetatable(L, LUA_BLIT_SIZE);
    return 1;
}

static int size_delete(lua_State* L){
    reinterpret_cast<Size*>(lua_touserdata(L, 1))->~Size();
    lua_pop(L, 1);
    return 0;
}

static int size_index(lua_State* L){
    int nargs = lua_gettop(L);
    Size *size = reinterpret_cast<Size*>(lua_touserdata(L, 1));
    std::string method = luaL_checkstring(L, 2);
    if (nargs == 3) {
        uint32_t value = luaL_checknumber(L, 3);
        lua_pop(L, nargs);
        if(method == "w") {size->w = value; return 0;}
        if(method == "h") {size->h = value; return 0;}
    }
    lua_pop(L, nargs);
    if(method == "w") {lua_pushnumber(L, size->w); return 1;}
    if(method == "h") {lua_pushnumber(L, size->h); return 1;}

    luaL_error(L, "Unknown property or method `%s` on %s", method.c_str(), LUA_BLIT_SIZE);
    return 0;
}

Size* lua_blit_checksize(lua_State *L, int arg) {
    auto size = luaL_checkudata(L, arg, LUA_BLIT_SIZE);
    return reinterpret_cast<Size*>(size);
}

void lua_blit_register_size(lua_State *L) {
    lua_register(L, LUA_BLIT_SIZE, size_new);
    luaL_newmetatable(L, LUA_BLIT_SIZE);
    lua_pushcfunction(L, size_delete); lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, size_index); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, size_index); lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
}