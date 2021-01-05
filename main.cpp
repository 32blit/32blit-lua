#include "main.hpp"
#include "lua/lua.hpp"

using namespace blit;

lua_State *L;
bool has_update = true;
bool has_render = true;

#ifndef _off_t
#define _off_t int
#endif

extern "C" {
    int _close(int) {return 0;}
    int _fstat(int, struct stat *) {return 0;}
    int _isatty(int) {return 0;}
    _off_t _lseek(int, _off_t, int) {return 0;}
    int _read(int, void *, size_t) {return 0;}
    int _write(int, const void *, size_t) {return 0;}
    int _now() {return blit::now();};
}


int lua_setPixel(lua_State* L) {
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

int lua_debug(lua_State* L) {
    int nargs = lua_gettop(L);

    for (int i = 1; i <= nargs; i++) {
        if (lua_isstring(L, i)) {
            const char *str = lua_tostring(L, i);
            blit::debugf(str);
        }
    }

    return 0;
}

void init() {
    set_screen_mode(ScreenMode::hires);
    screen.pen = Pen(0, 0, 0, 255);
    screen.clear();

    L = luaL_newstate();
    luaL_openlibs(L);

    luaL_Reg lua_c_functions[] = {
        {"debug", lua_debug},
        {"pixel", lua_setPixel},
        {NULL, NULL}
    };

    lua_getglobal(L, "_G");
    luaL_setfuncs(L, lua_c_functions, 0);
    lua_pop(L, 1);

    luaL_loadstring(L,
        "function init()\n"
        "   debug(\"hello world\")\n"
        "end\n"
        "\n"
        "function update(time)\n"
        "end\n"
        "\n"
        "function render(time)\n"
        "   pixel(10, 10, 0, 255, 0)\n"
        "   pixel(11, 10, 0, 255, 0)\n"
        "   pixel(10, 11, 0, 255, 0)\n"
        "   pixel(11, 11, 0, 255, 0)\n"
        "   local x = math.random(0, 319)\n"
        "   local y = math.random(0, 239)\n"
        "   pixel(x, y, 0, 0, 255)\n"
        "   debug(time)\n"
        "end\n"
    );

    // Super important priming call that makes stuff not explode
    lua_pcall(L, 0, 0, 0);

    lua_getglobal(L, "init");
    if(lua_isfunction(L, lua_gettop(L))){
        if(lua_pcall(L, 0, 0, 0) != 0){
            blit::debugf("Error running function `setup`: %s\n", lua_tostring(L, -1));
        }
    }

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
}

void update(uint32_t time) {
    if(!has_update) return;
    lua_getglobal(L, "update");
    lua_pushnumber(L, time);
    if(lua_pcall(L, 1, 0, 0) != 0){
        blit::debugf("Error running function `update`: %s\n", lua_tostring(L, -1));
    }
}
