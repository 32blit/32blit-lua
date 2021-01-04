#include "main.hpp"
#include "lua/lua.hpp"

using namespace blit;

lua_State *L;
bool has_update = true;
bool has_render = true;

int lua_debug(lua_State* L) {
    int nargs = lua_gettop(L);

    for (int i = 1; i <= nargs; i++) {
        if (lua_isstring(L, i)) {
            const char *str = lua_tostring(L, i);
            debugf(str);
        }
    }

    return 0;
}

void init() {
    set_screen_mode(ScreenMode::hires);
    L = luaL_newstate();
    luaL_openlibs(L);

    luaL_Reg lua_c_functions[] = {
        {"debug", lua_debug},
        {NULL, NULL}
    };

    lua_getglobal(L, "_G");
    luaL_setfuncs(L, lua_c_functions, 0);
    lua_pop(L, 1);

    luaL_loadstring(L,
        "function init()\n"
        "   print(\"hello world\")\n"
        "end\n"
        "\n"
        "function update(time)\n"
        "end\n"
        "\n"
        "function render(time)\n"
        "   print(time)\n"
        "end\n"
    );

    // Super important priming call that makes stuff not explode
    lua_pcall(L, 0, 0, 0);

    lua_getglobal(L, "init");
    if(lua_isfunction(L, lua_gettop(L))){
        if(lua_pcall(L, 0, 0, 0) != 0){
            debugf("Error running function `setup`: %s", lua_tostring(L, -1));
        }
    }

    lua_getglobal(L, "update");
    if(!lua_isfunction(L, lua_gettop(L))){
        debugf("Error `update` is not defined");
        has_update = false;
    }

    lua_getglobal(L, "render");
    if(!lua_isfunction(L, lua_gettop(L))){
        debugf("Error `render` is not defined");
        has_render = false;
    }
}

void render(uint32_t time) {
    if(!has_render) return;
    lua_getglobal(L, "render");
    lua_pushnumber(L, time);
    if(lua_pcall(L, 1, 0, 0) != 0){
        debugf("Error running function `render`: %s", lua_tostring(L, -1));
    }
}

void update(uint32_t time) {
    if(!has_update) return;
    lua_getglobal(L, "update");
    lua_pushnumber(L, time);
    if(lua_pcall(L, 1, 0, 0) != 0){
        debugf("Error running function `update`: %s", lua_tostring(L, -1));
    }
}
