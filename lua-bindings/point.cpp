#include "../luablitlib.hpp"

using namespace blit;

static int point_new(lua_State* L){
    int nargs = lua_gettop(L);
    int32_t x = 0;
    int32_t y = 0;
    if(nargs == 1) {
        Vec2 *vec = lua_blit_checkvec2(L, 1);
        x = (int32_t)vec->x;
        y = (int32_t)vec->y;
    }
    else {
        x = (int32_t)luaL_checknumber(L, 1);
        y = (int32_t)luaL_checknumber(L, 2);
    }
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
    std::string_view method = luaL_checkstring(L, 2);
    if (nargs == 3) {
        uint32_t value = luaL_checknumber(L, 3);
        lua_pop(L, nargs);
        if(method == "x") {point->x = value; return 0;}
        if(method == "y") {point->y = value; return 0;}
    }
    lua_pop(L, nargs);
    if(method == "x") {lua_pushnumber(L, point->x); return 1;}
    if(method == "y") {lua_pushnumber(L, point->y); return 1;}

    luaL_error(L, "Unknown property or method `%s` on %s", method.data(), LUA_BLIT_POINT);
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
    lua_pushcfunction(L, point_index); lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
}