#include "../luablitlib.hpp"

using namespace blit;

void input_table(lua_State *L, uint32_t input) {
    lua_newtable(L);
    lua_pushboolean(L, input & Button::A);
    lua_setfield(L, -2, "A");
    lua_pushboolean(L, input & Button::B);
    lua_setfield(L, -2, "B");
    lua_pushboolean(L, input & Button::X);
    lua_setfield(L, -2, "X");
    lua_pushboolean(L, input & Button::Y);
    lua_setfield(L, -2, "Y");

    lua_pushboolean(L, input & Button::DPAD_UP);
    lua_setfield(L, -2, "UP");
    lua_pushboolean(L, input & Button::DPAD_DOWN);
    lua_setfield(L, -2, "DOWN");
    lua_pushboolean(L, input & Button::DPAD_LEFT);
    lua_setfield(L, -2, "LEFT");
    lua_pushboolean(L, input & Button::DPAD_RIGHT);
    lua_setfield(L, -2, "RIGHT");
}

static int input_index(lua_State* L){
    int nargs = lua_gettop(L);
    std::string_view method = luaL_checkstring(L, 2);

    if(method == "joystick") {lua_blit_pushvec2(L, joystick); return 1;}
    if(method == "tilt") {lua_blit_pushvec3(L, tilt); return 1;}
    if(method == "state") {
        input_table(L, buttons.state);
        return 1;
    }
    if(method == "pressed") {
        input_table(L, buttons.pressed);
        return 1;
    }
    if(method == "released") {
        input_table(L, buttons.released);
        return 1;
    }

    luaL_error(L, "Unknown property or method `%s` on %s", std::string(method).c_str(), "input");
    return 0;
}

static const luaL_Reg input_funcs[] = {
    {NULL, NULL}
};

static const luaL_Reg screen_meta_funcs[] = {
    {"__index", input_index},
    {"__newindex", input_index},
    {NULL, NULL}
};

void lua_blit_setup_input(lua_State *L) {
    luaL_newmetatable(L, "input");
    lua_pushvalue(L, -1);
    luaL_setfuncs(L, input_funcs, 0);
    luaL_newlib(L, screen_meta_funcs);
    lua_setmetatable(L, -2);
    lua_setglobal(L, "input");
}