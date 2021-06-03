#include "../luablitlib.hpp"

using namespace blit;

static int point_new(lua_State* L){
    int nargs = lua_gettop(L);
    int32_t x = 0;
    int32_t y = 0;
    if(nargs == 1) {
        Point *vec = lua_blit_checkpoint(L, 1);
        x = (int32_t)vec->x;
        y = (int32_t)vec->y;
    }
    else {
        x = (int32_t)luaL_checknumber(L, 1);
        y = (int32_t)luaL_checknumber(L, 2);
    }
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
        int32_t value = luaL_checknumber(L, 3);
        if(method == "x") {point->x = value; return 0;}
        if(method == "y") {point->y = value; return 0;}
    }
    if(method == "x") {lua_pushnumber(L, point->x); return 1;}
    if(method == "y") {lua_pushnumber(L, point->y); return 1;}

    luaL_error(L, "Unknown property or method `%s` on %s", method.data(), LUA_BLIT_POINT);
    return 0;
}

static int point_add(lua_State *L){
    Point *point_a = reinterpret_cast<Point*>(lua_touserdata(L, 1));

    auto point_b = reinterpret_cast<Point *>(luaL_testudata(L, 2, LUA_BLIT_POINT));
    if(point_b) {
        lua_blit_pushpoint(L, *point_a + *point_b);
        return 1;
    }

    auto vec_b = reinterpret_cast<Vec2 *>(luaL_testudata(L, 2, LUA_BLIT_VEC2));
    if(vec_b) {
        lua_blit_pushpoint(L, *point_a + *vec_b);
        return 1;
    }
}

static int point_mul(lua_State *L){
    Point *point_a = reinterpret_cast<Point*>(lua_touserdata(L, 1));
    lua_Integer scalar = luaL_checkinteger(L, 2);
    lua_blit_pushpoint(L, *point_a * scalar);
    return 1;
}

static int point_div(lua_State *L){
    Point *point_a = reinterpret_cast<Point*>(lua_touserdata(L, 1));
    lua_Integer scalar = luaL_checkinteger(L, 2);
    lua_blit_pushpoint(L, *point_a / scalar);
    return 1;
}

static int point_eq(lua_State *L){
    Point *point_a = reinterpret_cast<Point*>(lua_touserdata(L, 1));
    Point *point_b = reinterpret_cast<Point*>(lua_touserdata(L, 2));
    lua_pushboolean(L, *point_a == *point_b);
    return 1;
}

static int point_tostring(lua_State *L){
    Point *point_a = reinterpret_cast<Point*>(lua_touserdata(L, 1));
    std::string repr = std::to_string(point_a->x) + " " + std::to_string(point_a->y);
    lua_pushstring(L, repr.c_str());
    return 1;
}

static int point_concat(lua_State *L){
    size_t len;
    auto ptr = luaL_checklstring(L, 1, &len);
    auto message = std::string_view(ptr, len);
    Point *point = reinterpret_cast<Point*>(lua_touserdata(L, 2));
    std::string repr = std::string(message) + std::to_string(point->x) + " " + std::to_string(point->y);
    lua_pushstring(L, repr.c_str());
    return 1;
}

void lua_blit_pushpoint(lua_State* L, Point p) {
    new(lua_newuserdata(L, sizeof(Point))) Point(p);
    luaL_setmetatable(L, LUA_BLIT_POINT);
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

    lua_pushcfunction(L, point_add); lua_setfield(L, -2, "__add");
    lua_pushcfunction(L, point_mul); lua_setfield(L, -2, "__mul");
    lua_pushcfunction(L, point_div); lua_setfield(L, -2, "__div");
    lua_pushcfunction(L, point_eq); lua_setfield(L, -2, "__eq");

    lua_pushcfunction(L, point_tostring); lua_setfield(L, -2, "__tostring");
    lua_pushcfunction(L, point_concat); lua_setfield(L, -2, "__concat"); // Is this, overkill?
    lua_pop(L, 1);
}