#include "../luablitlib.hpp"

using namespace blit;

static int rect_new(lua_State* L){
    int nargs = lua_gettop(L);
    if(nargs == 2){ // 2 args = Point and Size
        Point *point = lua_blit_checkpoint(L, 1);
        Size *size = lua_blit_checksize(L, 2);
        lua_blit_pushrect(L, Rect(*point, *size));
    }
    else if(nargs == 4){ // 4 args = x, y, w and h
        int x = (int32_t)luaL_checknumber(L, 1);
        int y = (int32_t)luaL_checknumber(L, 2);
        int w = (int32_t)luaL_checknumber(L, 3);
        int h = (int32_t)luaL_checknumber(L, 4);
        lua_blit_pushrect(L, Rect(x, y, w, h));
    }
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
    lua_pushboolean(L, result);
    return 1;
}

static int rect_intersects(lua_State* L){
    int nargs = lua_gettop(L);
    Rect *rect = reinterpret_cast<Rect*>(lua_touserdata(L, 1));
    Rect *test = lua_blit_checkrect(L, 2);
    bool result = rect->intersects(*test);
    lua_pushboolean(L, result);
    return 1;
}

static int rect_intersection(lua_State* L){
    int nargs = lua_gettop(L);
    Rect *rect = reinterpret_cast<Rect*>(lua_touserdata(L, 1));
    Rect *test = lua_blit_checkrect(L, 2);
    Rect result = rect->intersection(*test);
    lua_blit_pushrect(L, result);
    return 1;
}

static int rect_deflate(lua_State* L){
    int nargs = lua_gettop(L);
    Rect *rect = reinterpret_cast<Rect*>(lua_touserdata(L, 1));
    lua_Integer amount = luaL_checkinteger(L, 2);
    rect->deflate(amount);
    return 0;
}

static int rect_inflate(lua_State* L){
    int nargs = lua_gettop(L);
    Rect *rect = reinterpret_cast<Rect*>(lua_touserdata(L, 1));
    lua_Integer amount = luaL_checkinteger(L, 2);
    rect->inflate(amount);
    return 0;
}

static int rect_clamp(lua_State* L){
    int nargs = lua_gettop(L);
    Rect *rect = reinterpret_cast<Rect*>(lua_touserdata(L, 1));
    Point *point = lua_blit_checkpoint(L, 2);
    Point result = rect->clamp(*point);
    lua_blit_pushpoint(L, result);
    return 1;
}

static int rect_index(lua_State* L){
    int nargs = lua_gettop(L);
    Rect *rect = reinterpret_cast<Rect*>(lua_touserdata(L, 1));
    std::string_view method = luaL_checkstring(L, 2);
    if(nargs == 3) {
        int32_t value = luaL_checknumber(L, 3);
        if(method == "x") {rect->x = value; return 0;}
        if(method == "y") {rect->y = value; return 0;}
        if(method == "w") {rect->w = value; return 0;}
        if(method == "h") {rect->h = value; return 0;}
    }
    if(method == "x") {lua_pushnumber(L, rect->x); return 1;}
    if(method == "y") {lua_pushnumber(L, rect->y); return 1;}
    if(method == "w") {lua_pushnumber(L, rect->w); return 1;}
    if(method == "h") {lua_pushnumber(L, rect->h); return 1;}
    if(method == "center") {lua_blit_pushpoint(L, rect->center()); return 1;}
    if(method == "size") {
        lua_blit_pushsize(L, rect->size());
        return 1;
    }
    if(method == "empty") {lua_pushboolean(L, rect->empty()); return 1;}
    if(method == "contains") {lua_pushcfunction(L, rect_contains); return 1;}
    if(method == "intersects") {lua_pushcfunction(L, rect_intersects); return 1;}
    if(method == "intersection") {lua_pushcfunction(L, rect_intersection); return 1;}
    if(method == "deflate") {lua_pushcfunction(L, rect_deflate); return 1;}
    if(method == "inflate") {lua_pushcfunction(L, rect_inflate); return 1;}
    if(method == "clamp") {lua_pushcfunction(L, rect_clamp); return 1;}

    luaL_error(L, "Unknown property or method `%s` on %s", method.data(), LUA_BLIT_RECT);
    return 0;
}

void lua_blit_pushrect(lua_State* L, Rect p) {
    new(lua_newuserdata(L, sizeof(Rect))) Rect(p);
    luaL_setmetatable(L, LUA_BLIT_RECT);
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