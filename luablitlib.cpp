/*
** $Id: luablitlib.cpp $
** Lua extension for 32blit engine
*/
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "luablitlib.hpp"

#include "graphics/color.hpp"

using namespace blit;

static int clear(lua_State *L) {
    screen.clear();
    return 0;
}

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

static int h_span(lua_State *L) {
    int nargs = lua_gettop(L);
    Point *p = lua_blit_checkpoint(L, 1);
    auto c = luaL_checkinteger(L, 2);
    screen.h_span(*p, c);
    lua_pop(L, nargs);
    return 0;
}

static int v_span(lua_State *L) {
    int nargs = lua_gettop(L);
    Point *p = lua_blit_checkpoint(L, 1);
    auto c = luaL_checkinteger(L, 2);
    screen.v_span(*p, c);
    lua_pop(L, nargs);
    return 0;
}

static int line(lua_State *L) {
    int nargs = lua_gettop(L);
    Point *a = lua_blit_checkpoint(L, 1);
    Point *b = lua_blit_checkpoint(L, 2);
    screen.line(*a, *b);
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

static int hsv_to_rgba(lua_State *L) {
    float h = luaL_checknumber(L, 1);
    float s = luaL_checknumber(L, 2);
    float v = luaL_checknumber(L, 3);

    auto tmp = blit::hsv_to_rgba(h, s, v);
    new(lua_newuserdata(L, sizeof(Pen))) Pen(tmp);
    luaL_setmetatable(L, LUA_BLIT_PEN);
    return 1;
}

void lua_blit_update_state(lua_State *L) {
    lua_pushnumber(L, buttons.pressed);
    lua_setglobal(L, "pressed");
    lua_pushnumber(L, buttons.released);
    lua_setglobal(L, "released");
    lua_pushnumber(L, buttons.state);
    lua_setglobal(L, "state");

    new(lua_newuserdata(L, sizeof(Vec2))) Vec2(joystick);
    luaL_setmetatable(L, LUA_BLIT_VEC2);
    lua_setglobal(L, "joystick");
}

static const luaL_Reg funcs[] = {
    {"pen", pen},
    {"pixel", pixel},
    {"h_span", h_span},
    {"v_span", v_span},
    {"line", line},
    {"rectangle", rectangle},
    {"clear", clear},
    {"debug", debug},
    {"hsv_to_rgba", hsv_to_rgba},
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

    lua_pushnumber(L, Button::A);
    lua_setglobal(L, "A");
    lua_pushnumber(L, Button::B);
    lua_setglobal(L, "B");
    lua_pushnumber(L, Button::X);
    lua_setglobal(L, "X");
    lua_pushnumber(L, Button::Y);
    lua_setglobal(L, "Y");
    lua_pushnumber(L, Button::DPAD_UP);
    lua_setglobal(L, "UP");
    lua_pushnumber(L, Button::DPAD_DOWN);
    lua_setglobal(L, "DOWN");
    lua_pushnumber(L, Button::DPAD_LEFT);
    lua_setglobal(L, "LEFT");
    lua_pushnumber(L, Button::DPAD_RIGHT);
    lua_setglobal(L, "RIGHT");
    lua_pushnumber(L, Button::MENU);
    lua_setglobal(L, "MENU");

    return 1;
}