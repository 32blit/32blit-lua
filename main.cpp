#include "main.hpp"
#include "stdio-wrap.hpp"
#include "lua/lua.hpp"
#include "luablitlib.hpp"

using namespace blit;

lua_State *L;
bool has_update = true;
bool has_render = true;

void init() {
    set_screen_mode(ScreenMode::hires);
    screen.pen = Pen(0, 0, 0, 255);
    screen.clear();

    L = luaL_newstate();
    luaL_openlibs(L);
    luaL_requiref(L, "blit", luaopen_blit, 1);

    auto launchPath = blit::get_launch_path();
    if(!launchPath) {
        launchPath = "main.lua";
    }
    luaL_loadfile(L, launchPath);

    // Super important priming call that makes stuff not explode
    if(lua_pcall(L, 0, 0, 0) != 0){
        blit::debugf("Error loading %s: %s\n", launchPath, lua_tostring(L, -1));
    }

    lua_getglobal(L, "init");
    if(lua_isfunction(L, lua_gettop(L))){
        if(lua_pcall(L, 0, 0, 0) != 0){
            blit::debugf("Error running function `init`: %s\n", lua_tostring(L, -1));
        }
    }
    lua_gc(L, LUA_GCCOLLECT, 0);

    lua_getglobal(L, "update");
    if(!lua_isfunction(L, lua_gettop(L))){
        blit::debugf("Error `update` is not defined\n");
        has_update = false;
    }

    lua_getglobal(L, "render");
    if(!lua_isfunction(L, lua_gettop(L))){
        blit::debugf("Error `render` is not defined\n");
        has_render = false;
    }
}

void render(uint32_t time) {
    if(!has_render) return;
    lua_getglobal(L, "render");
    lua_pushnumber(L, time);
    if(lua_pcall(L, 1, 0, 0) != 0){
        blit::debugf("Error running function `render`: %s\n", lua_tostring(L, -1));
    }
    lua_gc(L, LUA_GCCOLLECT, 0);
}

void update(uint32_t time) {
    if(!has_update) return;
    lua_getglobal(L, "update");
    lua_pushnumber(L, time);
    if(lua_pcall(L, 1, 0, 0) != 0){
        blit::debugf("Error running function `update`: %s\n", lua_tostring(L, -1));
    }
}
