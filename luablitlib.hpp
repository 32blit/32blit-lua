#define luablitlib_cpp
#define LUA_LIB

#include "32blit.hpp"


#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "lua/lua.hpp"
#include "lua/lauxlib.h"

LUAMOD_API int luaopen_blit (lua_State *L);