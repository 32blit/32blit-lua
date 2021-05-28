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

static int now(lua_State *L) {
    lua_pushinteger(L, blit::now());
    return 1;
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
    lua_blit_pushpen(L, tmp);
    return 1;
}

void lua_blit_update_state(lua_State *L) {
    // use "buttons.pressed & Button.RIGHT ~= 0"

    lua_newtable(L);
        lua_pushnumber(L, buttons.pressed);
        lua_setfield(L, -2, "pressed");
        lua_pushnumber(L, buttons.released);
        lua_setfield(L, -2, "released");
        lua_pushnumber(L, buttons.state);
        lua_setfield(L, -2, "state");
    lua_setglobal(L, "buttons");

    new(lua_newuserdata(L, sizeof(Vec2))) Vec2(joystick);
    luaL_setmetatable(L, LUA_BLIT_VEC2);
    lua_setglobal(L, "joystick");
}

static const luaL_Reg blit_funcs[] = {
    {"now", now},
    {"debug", debug},
    {"hsv_to_rgba", hsv_to_rgba},
    {NULL, NULL}
};

LUAMOD_API int luaopen_blit (lua_State *L) {
    luaL_newlib(L, blit_funcs);
    return 1;
}

void lua_blit_setup_globals (lua_State *L) {
    lua_blit_setup_screen(L);

    // We'll allow all basic types in the global namespace
    lua_blit_register_point(L);
    lua_blit_register_pen(L);
    lua_blit_register_vec2(L);
    lua_blit_register_size(L);
    lua_blit_register_rect(L);
    //lua_blit_register_sprites(L);
    lua_blit_register_timer(L);

    // And fonts, too
    lua_pushlightuserdata(L, (void *)(&minimal_font));
    lua_setglobal(L, "minimal_font");
    lua_pushlightuserdata(L, (void *)(&fat_font));
    lua_setglobal(L, "fat_font");
    lua_pushlightuserdata(L, (void *)(&outline_font));
    lua_setglobal(L, "outline_font");

    // ScreenMode enum
    lua_newtable(L);
        lua_pushnumber(L, ScreenMode::lores);
        lua_setfield(L, -2, "lores");
        lua_pushnumber(L, ScreenMode::hires);
        lua_setfield(L, -2, "hires");
        lua_pushnumber(L, ScreenMode::hires_palette);
        lua_setfield(L, -2, "hires_palette");
    lua_setglobal(L, "ScreenMode");

    // Button enum
    lua_newtable(L);
        lua_pushnumber(L, Button::A);
        lua_setfield(L, -2, "A");
        lua_pushnumber(L, Button::B);
        lua_setfield(L, -2, "B");
        lua_pushnumber(L, Button::X);
        lua_setfield(L, -2, "X");
        lua_pushnumber(L, Button::Y);
        lua_setfield(L, -2, "Y");
        lua_pushnumber(L, Button::DPAD_UP);
        lua_setfield(L, -2, "UP");
        lua_pushnumber(L, Button::DPAD_DOWN);
        lua_setfield(L, -2, "DOWN");
        lua_pushnumber(L, Button::DPAD_LEFT);
        lua_setfield(L, -2, "LEFT");
        lua_pushnumber(L, Button::DPAD_RIGHT);
        lua_setfield(L, -2, "RIGHT");
        lua_pushnumber(L, Button::MENU);
        lua_setfield(L, -2, "MENU");
    lua_setglobal(L, "Button");

    // TextAlign enum
    lua_newtable(L);
        lua_pushinteger(L, TextAlign::left);
        lua_setfield(L, -2, "left");
        lua_pushinteger(L, TextAlign::right);
        lua_setfield(L, -2, "right");
        lua_pushinteger(L, TextAlign::top);
        lua_setfield(L, -2, "top");
        lua_pushinteger(L, TextAlign::bottom);
        lua_setfield(L, -2, "bottom");
        lua_pushinteger(L, TextAlign::center_h);
        lua_setfield(L, -2, "center_h");
        lua_pushinteger(L, TextAlign::center_v);
        lua_setfield(L, -2, "center_v");
        
        lua_pushinteger(L, TextAlign::bottom_left);
        lua_setfield(L, -2, "bottom_left");
        lua_pushinteger(L, TextAlign::bottom_right);
        lua_setfield(L, -2, "bottom_right");
        lua_pushinteger(L, TextAlign::top_left);
        lua_setfield(L, -2, "top_left");
        lua_pushinteger(L, TextAlign::top_right);
        lua_setfield(L, -2, "top_right");
        lua_pushinteger(L, TextAlign::center_center);
        lua_setfield(L, -2, "center_center");
        lua_pushinteger(L, TextAlign::center_left);
        lua_setfield(L, -2, "center_left");
        lua_pushinteger(L, TextAlign::center_right);
        lua_setfield(L, -2, "center_right");
        lua_pushinteger(L, TextAlign::top_center);
        lua_setfield(L, -2, "top_center");
        lua_pushinteger(L, TextAlign::bottom_center);
        lua_setfield(L, -2, "bottom_center");
    lua_setglobal(L, "TextAlign");
}