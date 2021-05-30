#include "../luablitlib.hpp"

using namespace blit;

static int size_new(lua_State* L){
    int32_t x = (int32_t)luaL_checknumber(L, 1);
    int32_t y = (int32_t)luaL_checknumber(L, 2);
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
    std::string_view method = luaL_checkstring(L, 2);
    if (nargs == 3) {
        uint32_t value = luaL_checknumber(L, 3);
        if(method == "w") {size->w = value; return 0;}
        if(method == "h") {size->h = value; return 0;}
    }
    if(method == "w") {lua_pushnumber(L, size->w); return 1;}
    if(method == "h") {lua_pushnumber(L, size->h); return 1;}

    luaL_error(L, "Unknown property or method `%s` on %s", method.data(), LUA_BLIT_SIZE);
    return 0;
}

static int size_mul(lua_State *L){
    Size *size_a = reinterpret_cast<Size*>(lua_touserdata(L, 1));
    lua_Integer scalar = luaL_checkinteger(L, 2);
    lua_blit_pushsize(L, *size_a * scalar);
    return 1;
}

static int size_div(lua_State *L){
    Size *size_a = reinterpret_cast<Size*>(lua_touserdata(L, 1));
    lua_Integer scalar = luaL_checkinteger(L, 2);
    lua_blit_pushsize(L, *size_a / scalar);
    return 1;
}

static int size_eq(lua_State *L){
    Size *size_a = reinterpret_cast<Size*>(lua_touserdata(L, 1));
    Size *size_b = reinterpret_cast<Size*>(lua_touserdata(L, 2));
    lua_pushboolean(L, *size_a == *size_b);
    return 1;
}

void lua_blit_pushsize(lua_State* L, Size p) {
    new(lua_newuserdata(L, sizeof(Size))) Size(p);
    luaL_setmetatable(L, LUA_BLIT_SIZE);
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

    lua_pushcfunction(L, size_mul); lua_setfield(L, -2, "__mul");
    lua_pushcfunction(L, size_div); lua_setfield(L, -2, "__div");
    lua_pushcfunction(L, size_eq); lua_setfield(L, -2, "__eq");
    lua_pop(L, 1);
}