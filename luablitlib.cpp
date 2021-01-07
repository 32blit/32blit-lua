/*
** $Id: luablitlib.cpp $
** Lua extension for 32blit engine
*/
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "luablitlib.hpp"

using namespace blit;

static int pen(lua_State *L) {
    int nargs = lua_gettop(L);
    Pen *p = lua_blit_checkpen(L, 1);
    lua_pop(L, nargs);
    screen.pen = *p;
    return 0;
}

static int pixel(lua_State *L) {
    int nargs = lua_gettop(L);
    Point *p = lua_blit_checkpoint(L, 1);
    screen.pixel(*p);
    lua_pop(L, nargs);
    return 0;
}

static int rectangle(lua_State *L) {
    int nargs = lua_gettop(L);
    Rect *r = lua_blit_checkrect(L, 1);
    screen.rectangle(*r);
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
    {"rectangle", rectangle},
    {"debug", debug},
    {NULL, NULL}
};

LUAMOD_API int luaopen_blit (lua_State *L) {
    luaL_newlib(L, funcs);

    lua_blit_register_point(L);
    lua_blit_register_pen(L);
    lua_blit_register_vec2(L);
    lua_blit_register_size(L);
    lua_blit_register_rect(L);
    lua_blit_register_sprites(L);

    return 1;
}