#include "../luablitlib.hpp"

using namespace blit;

static int rect_new(lua_State* L){
    int nargs = lua_gettop(L);
    if(nargs == 2){ // 2 args = Point and Size
        Point *point = lua_blit_checkpoint(L, 1);
        Size *size = lua_blit_checksize(L, 2);
        lua_pop(L, nargs);
        new(lua_newuserdata(L, sizeof(Rect))) Rect(*point, *size);
    }
    else if(nargs == 4){ // 4 args = x, y, w and h
        int x = (int32_t)luaL_checknumber(L, 1);
        int y = (int32_t)luaL_checknumber(L, 2);
        int w = (int32_t)luaL_checknumber(L, 3);
        int h = (int32_t)luaL_checknumber(L, 4);
        lua_pop(L, nargs);
        new(lua_newuserdata(L, sizeof(Rect))) Rect(x, y, w, h);
    }
    luaL_setmetatable(L, LUA_BLIT_RECT);
    return 1;
}

static int rect_delete(lua_State* L){
    reinterpret_cast<Rect*>(lua_touserdata(L, 1))->~Rect();
    lua_pop(L, 1);
    return 0;
}

static int rect_contains(lua_State* L){
    int nargs = lua_gettop(L);
    Rect *rect = reinterpret_cast<Rect*>(lua_touserdata(L, 1));
    Point *point = lua_blit_checkpoint(L, 2);
    bool result = rect->contains(*point);
    lua_pop(L, nargs);
    lua_pushboolean(L, result);
    return 1;
}

static int rect_intersects(lua_State* L){
    int nargs = lua_gettop(L);
    Rect *rect = reinterpret_cast<Rect*>(lua_touserdata(L, 1));
    Rect *test = lua_blit_checkrect(L, 2);
    bool result = rect->intersects(*test);
    lua_pop(L, nargs);
    lua_pushboolean(L, result);
    return 1;
}

static int rect_index(lua_State* L){
    int nargs = lua_gettop(L);
    Rect *rect = reinterpret_cast<Rect*>(lua_touserdata(L, 1));
    std::string_view method = luaL_checkstring(L, 2);
    lua_pop(L, nargs);
    if(nargs == 3) {
        int32_t value = luaL_checknumber(L, 3);
        lua_pop(L, nargs);
        if(method == "x") {rect->x = value; return 0;}
        if(method == "y") {rect->y = value; return 0;}
        if(method == "w") {rect->w = value; return 0;}
        if(method == "h") {rect->h = value; return 0;}
    }
    if(method == "x") {lua_pushnumber(L, rect->x); return 1;}
    if(method == "y") {lua_pushnumber(L, rect->y); return 1;}
    if(method == "w") {lua_pushnumber(L, rect->w); return 1;}
    if(method == "h") {lua_pushnumber(L, rect->h); return 1;}
    if(method == "contains") {lua_pushcfunction(L, rect_contains); return 1;}
    if(method == "intersects") {lua_pushcfunction(L, rect_intersects); return 1;}

    luaL_error(L, "Unknown property or method `%s` on %s", method.data(), LUA_BLIT_RECT);
    return 0;
}

Rect* lua_blit_checkrect(lua_State *L, int arg) {
    auto p = luaL_checkudata(L, arg, LUA_BLIT_RECT);
    return reinterpret_cast<Rect*>(p);
}

void lua_blit_register_rect(lua_State *L) {
    lua_register(L, LUA_BLIT_RECT, rect_new);
    luaL_newmetatable(L, LUA_BLIT_RECT);
    lua_pushcfunction(L, rect_delete); lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, rect_index); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, rect_index); lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
}