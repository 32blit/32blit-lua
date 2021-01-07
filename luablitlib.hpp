#pragma once
#define luablitlib_cpp
#define LUA_LIB

#include "32blit.hpp"
#include "lua/lua.hpp"
#include "lua/lauxlib.h"

// Lua names for userdata bindings
#define LUA_BLIT_POINT "Point"
#define LUA_BLIT_PEN "Pen"
#define LUA_BLIT_VEC2 "Vec2"
#define LUA_BLIT_SIZE "Size"
#define LUA_BLIT_RECT "Rect"

LUAMOD_API int luaopen_blit (lua_State *L);
void lua_blit_update_state(lua_State *L);

void lua_blit_register_point(lua_State *L);
blit::Point* lua_blit_checkpoint(lua_State *L, int arg);

void lua_blit_register_pen(lua_State *L);
blit::Pen* lua_blit_checkpen(lua_State *L, int arg);

void lua_blit_register_vec2(lua_State *L);
blit::Vec2* lua_blit_checkvec2(lua_State *L, int arg);

void lua_blit_register_size(lua_State *L);
blit::Size* lua_blit_checksize(lua_State *L, int arg);

void lua_blit_register_rect(lua_State *L);
blit::Rect* lua_blit_checkrect(lua_State *L, int arg);

void lua_blit_register_sprites(lua_State *L);