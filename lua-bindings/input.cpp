#include "../luablitlib.hpp"

using namespace blit;

Vec2* ud_joystick;
Vec3 *ud_tilt;

void lua_blit_setup_input(lua_State *L) {
    lua_newtable(L);
        lua_pushinteger(L, buttons.pressed);
        lua_setfield(L, -2, "pressed");
        lua_pushinteger(L, buttons.released);
        lua_setfield(L, -2, "released");
        lua_pushinteger(L, buttons.state);
        lua_setfield(L, -2, "state");
    lua_setglobal(L, "buttons");

    ud_joystick = new(lua_newuserdata(L, sizeof(Vec2))) Vec2(joystick);
    luaL_setmetatable(L, LUA_BLIT_VEC2);
    lua_setglobal(L, "joystick");

    ud_tilt = new(lua_newuserdata(L, sizeof(Vec3))) Vec3(tilt);
    luaL_setmetatable(L, LUA_BLIT_VEC3);
    lua_setglobal(L, "tilt");
}

void lua_blit_update_input(lua_State *L) {
    // use "buttons.pressed & Button.RIGHT ~= 0"

    lua_getglobal(L, "buttons");
        lua_pushinteger(L, buttons.pressed);
        lua_setfield(L, -2, "pressed");
        lua_pushinteger(L, buttons.released);
        lua_setfield(L, -2, "released");
        lua_pushinteger(L, buttons.state);
        lua_setfield(L, -2, "state");
    lua_pop(L, 1);

    *ud_joystick = joystick;
    *ud_tilt = tilt;
}