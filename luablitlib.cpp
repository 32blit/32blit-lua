/*
** $Id: luablitlib.cpp $
** Lua extension for 32blit engine
*/

#include "luablitlib.hpp"

using namespace blit;

static int sprite(lua_State *L) {
    int nargs = lua_gettop(L);
    unsigned int i = luaL_checknumber(L, 1);
    unsigned int x = luaL_checknumber(L, 2);
    unsigned int y = luaL_checknumber(L, 3);
    lua_pop(L, nargs);
    screen.sprite(i, Point(x, y));
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

static int pixel(lua_State *L) {
    int nargs = lua_gettop(L);
    unsigned int x = luaL_checknumber(L, 1);
    unsigned int y = luaL_checknumber(L, 2);
    unsigned int r = luaL_checknumber(L, 3);
    unsigned int g = luaL_checknumber(L, 4);
    unsigned int b = luaL_checknumber(L, 5);
    screen.pen = Pen(r, g, b, 255);
    screen.pixel(Point(x, y));
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
  {"pixel", pixel},
  {"debug", debug},
  {"load_sprites", load_sprites},
  {"sprite", sprite},
  {NULL, NULL}
};

LUAMOD_API int luaopen_blit (lua_State *L) {
  luaL_newlib(L, funcs);
  return 1;
}