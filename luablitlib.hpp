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
#define LUA_BLIT_VEC3 "Vec3"
#define LUA_BLIT_SIZE "Size"
#define LUA_BLIT_RECT "Rect"
#define LUA_BLIT_TIMER "Timer"
#define LUA_BLIT_SURFACE "Surface"
#define LUA_BLIT_PALETTE "Palette"

LUAMOD_API int luaopen_blit (lua_State *L);
void lua_blit_setup_globals (lua_State *L);
void lua_blit_update_state(lua_State *L);

void lua_blit_setup_screen (lua_State *L);
void lua_blit_setup_input(lua_State *L);

void lua_blit_register_point(lua_State *L);
void lua_blit_pushpoint(lua_State* L, blit::Point p);
blit::Point* lua_blit_checkpoint(lua_State *L, int arg);

void lua_blit_register_pen(lua_State *L);
void lua_blit_pushpen(lua_State *L, blit::Pen p);
blit::Pen* lua_blit_checkpen(lua_State *L, int arg);

void lua_blit_register_vec2(lua_State *L);
void lua_blit_pushvec2(lua_State* L, blit::Vec2 p);
blit::Vec2* lua_blit_checkvec2(lua_State *L, int arg);

void lua_blit_register_vec3(lua_State *L);
void lua_blit_pushvec3(lua_State* L, blit::Vec3 p);
blit::Vec3* lua_blit_checkvec3(lua_State *L, int arg);

void lua_blit_register_size(lua_State *L);
void lua_blit_pushsize(lua_State* L, blit::Size p);
blit::Size* lua_blit_checksize(lua_State *L, int arg);

void lua_blit_register_rect(lua_State *L);
void lua_blit_pushrect(lua_State* L, blit::Rect p);
blit::Rect* lua_blit_checkrect(lua_State *L, int arg);

void lua_blit_register_timer(lua_State *L);

struct Palette {
    blit::Pen *entries;
};
void lua_blit_register_palette(lua_State *L);
void lua_blit_pushpalette(lua_State* L, Palette *p);
void lua_blit_pushpalette(lua_State* L, blit::Pen *p);
Palette* lua_blit_checkpalette(lua_State *L, int arg);

void lua_blit_register_surface(lua_State *L);
void lua_blit_pushsurface(lua_State* L, blit::Surface s);
blit::Surface* lua_blit_checksurface(lua_State *L, int arg);