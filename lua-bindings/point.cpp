#include "../luablitlib.hpp"

using namespace blit;

static int point_new(lua_State* L){
    int nargs = lua_gettop(L);
    int32_t x = (int32_t)luaL_checknumber(L, 1);
    int32_t y = (int32_t)luaL_checknumber(L, 2);
    lua_pop(L, nargs);
    new(lua_newuserdata(L, sizeof(Point))) Point(x, y);
    luaL_setmetatable(L, LUA_BLIT_POINT);
    return 1;
}

static int point_delete(lua_State* L){
    reinterpret_cast<Point*>(lua_touserdata(L, 1))->~Point();
    lua_pop(L, 1);
    return 0;
}

static int point_index(lua_State* L){
    int nargs = lua_gettop(L);
    Point *point = reinterpret_cast<Point*>(lua_touserdata(L, 1));
    std::string method = luaL_checkstring(L, 2);
    lua_pop(L, nargs);
    if(method == "x") {lua_pushnumber(L, point->x); return 1;}
    if(method == "y") {lua_pushnumber(L, point->y); return 1;}
    return 0;
}

Point* lua_blit_checkpoint(lua_State *L, int arg) {
    auto point = luaL_checkudata(L, arg, LUA_BLIT_POINT);
    return reinterpret_cast<Point*>(point);
}

void lua_blit_register_point(lua_State *L) {
    lua_register(L, LUA_BLIT_POINT, point_new);
    luaL_newmetatable(L, LUA_BLIT_POINT);
    lua_pushcfunction(L, point_delete); lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, point_index); lua_setfield(L, -2, "__index");
    lua_pop(L, 1);
}