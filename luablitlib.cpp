/*
** $Id: luablitlib.cpp $
** Lua extension for 32blit engine
*/

#include "luablitlib.hpp"

#define LUA_BLIT_POINT "Point"
#define LUA_BLIT_PEN "Pen"

using namespace blit;

static int pen_new(lua_State* L){
    int32_t a = 255;
    int nargs = lua_gettop(L);
    int32_t r = (int32_t)luaL_checknumber(L, 1);
    int32_t g = (int32_t)luaL_checknumber(L, 2);
    int32_t b = (int32_t)luaL_checknumber(L, 3);
    if(nargs == 4){
        a = (int32_t)luaL_checknumber(L, r);
    }
    lua_pop(L, nargs);
    *reinterpret_cast<Pen**>(lua_newuserdata(L, sizeof(Pen*))) = new Pen(r, g, b, a);
    luaL_setmetatable(L, LUA_BLIT_PEN);
    return 1;
}

static int pen_delete(lua_State* L){
    delete *reinterpret_cast<Pen**>(lua_touserdata(L, 1));
    return 0;
}

static int pen_index(lua_State* L){
    int nargs = lua_gettop(L);
    Pen pen = **reinterpret_cast<Pen**>(lua_touserdata(L, 1));
    std::string method = luaL_checkstring(L, 2);
    lua_pop(L, nargs);
    nargs = 0;
    if(method == "r") {lua_pushnumber(L, pen.r); nargs=1;}
    if(method == "g") {lua_pushnumber(L, pen.g); nargs=1;}
    if(method == "b") {lua_pushnumber(L, pen.b); nargs=1;}
    if(method == "a") {lua_pushnumber(L, pen.a); nargs=1;}
    return nargs;
}

Pen lua_blit_checkpen(lua_State *L, int arg) {
    auto pen = luaL_checkudata(L, arg, LUA_BLIT_PEN);
    return **reinterpret_cast<Pen**>(pen);
}

static int point_new(lua_State* L){
    int nargs = lua_gettop(L);
    int32_t x = (int32_t)luaL_checknumber(L, 1);
    int32_t y = (int32_t)luaL_checknumber(L, 2);
    lua_pop(L, nargs);
    *reinterpret_cast<Point**>(lua_newuserdata(L, sizeof(Point*))) = new Point(x, y);
    luaL_setmetatable(L, LUA_BLIT_POINT);
    return 1;
}

static int point_delete(lua_State* L){
    delete *reinterpret_cast<Point**>(lua_touserdata(L, 1));
    return 0;
}

static int point_index(lua_State* L){
    int nargs = lua_gettop(L);
    Point point = **reinterpret_cast<Point**>(lua_touserdata(L, 1));
    std::string method = luaL_checkstring(L, 2);
    lua_pop(L, nargs);
    nargs = 0;
    if(method == "x") {lua_pushnumber(L, point.x); nargs=1;}
    if(method == "y") {lua_pushnumber(L, point.y); nargs=1;}
    return nargs;
}

Point lua_blit_checkpoint(lua_State *L, int arg) {
    auto point = luaL_checkudata(L, arg, LUA_BLIT_POINT);
    return **reinterpret_cast<Point**>(point);
}

static int sprite(lua_State *L) {
    int nargs = lua_gettop(L);
    unsigned int i = luaL_checknumber(L, 1);
    Point p = lua_blit_checkpoint(L, 2);
    lua_pop(L, nargs);
    screen.sprite(i, p);
    return 0;
}

static int load_sprites(lua_State *L) {
    int nargs = lua_gettop(L);
    std::string filename = luaL_checkstring(L, 1);
    lua_pop(L, nargs);
    if(screen.sprites != nullptr) {
        delete screen.sprites->data;
        delete screen.sprites->palette;
        delete screen.sprites;
        screen.sprites = nullptr;
    }
    screen.sprites = SpriteSheet::load(filename);
    return 0;
}

static int pen(lua_State *L) {
    int nargs = lua_gettop(L);
    Pen p = lua_blit_checkpen(L, 1);
    lua_pop(L, nargs);
    screen.pen = p;
    return 0;
}

static int pixel(lua_State *L) {
    int nargs = lua_gettop(L);
    Point p = lua_blit_checkpoint(L, 1);
    screen.pixel(p);
    lua_pop(L, nargs);
    return 0;
}

static int debug(lua_State* L) {
    int nargs = lua_gettop(L);

    for (int i = 1; i <= nargs; i++) {
        if (lua_isstring(L, i)) {
            const char *str = lua_tostring(L, i);
            blit::debugf(str);
            blit::debugf("\n");
        }
    }

    return 0;
}

static const luaL_Reg funcs[] = {
    {"pen", pen},
    {"pixel", pixel},
    {"debug", debug},
    {"load_sprites", load_sprites},
    {"sprite", sprite},
    {NULL, NULL}
};

LUAMOD_API int luaopen_blit (lua_State *L) {
    luaL_newlib(L, funcs);

    // Point
    lua_register(L, LUA_BLIT_POINT, point_new);
    luaL_newmetatable(L, LUA_BLIT_POINT);
    lua_pushcfunction(L, point_delete); lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, point_index); lua_setfield(L, -2, "__index");
    lua_pop(L, 1);

    // Pen
    lua_register(L, LUA_BLIT_PEN, pen_new);
    luaL_newmetatable(L, LUA_BLIT_PEN);
    lua_pushcfunction(L, pen_delete); lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, pen_index); lua_setfield(L, -2, "__index");
    lua_pop(L, 1);

    return 1;
}