#include "../luablitlib.hpp"

using namespace blit;

static int pen_new(lua_State* L){
    int a = 255;
    int nargs = lua_gettop(L);
    int r = (int32_t)luaL_checknumber(L, 1);
    int g = (int32_t)luaL_checknumber(L, 2);
    int b = (int32_t)luaL_checknumber(L, 3);
    if(nargs == 4){
        a = (int32_t)luaL_checknumber(L, r);
    }
    lua_pop(L, nargs);
    new(lua_newuserdata(L, sizeof(Pen))) Pen(r, g, b, a);
    luaL_setmetatable(L, LUA_BLIT_PEN);
    return 1;
}

static int pen_delete(lua_State* L){
    reinterpret_cast<Pen*>(lua_touserdata(L, 1))->~Pen();
    lua_pop(L, 1);
    return 0;
}

static int pen_index(lua_State* L){
    int nargs = lua_gettop(L);
    Pen *pen = reinterpret_cast<Pen*>(lua_touserdata(L, 1));
    std::string method = luaL_checkstring(L, 2);
    lua_pop(L, nargs);
    if(method == "r") {lua_pushnumber(L, pen->r); return 1;}
    if(method == "g") {lua_pushnumber(L, pen->g); return 1;}
    if(method == "b") {lua_pushnumber(L, pen->b); return 1;}
    if(method == "a") {lua_pushnumber(L, pen->a); return 1;}
    return 0;
}

Pen* lua_blit_checkpen(lua_State *L, int arg) {
    auto p = luaL_checkudata(L, arg, LUA_BLIT_PEN);
    return reinterpret_cast<Pen*>(p);
}

void lua_blit_register_pen(lua_State *L) {
    lua_register(L, LUA_BLIT_PEN, pen_new);
    luaL_newmetatable(L, LUA_BLIT_PEN);
    lua_pushcfunction(L, pen_delete); lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, pen_index); lua_setfield(L, -2, "__index");
    lua_pop(L, 1);
}