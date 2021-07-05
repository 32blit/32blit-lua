#include "../luablitlib.hpp"

using namespace blit;

Vec2* ud_joystick;
Vec3 *ud_tilt;

void input_state_table(lua_State *L, uint32_t input) {
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

    lua_pushboolean(L, input & Button::HOME);
    lua_setfield(L, -2, "HOME");
    lua_pushboolean(L, input & Button::MENU);
    lua_setfield(L, -2, "MENU");

    lua_pushboolean(L, input & Button::JOYSTICK);
    lua_setfield(L, -2, "JOYSTICK");
}

void lua_blit_setup_input(lua_State *L) {
    lua_newtable(L);
    lua_setglobal(L, "buttons");

    lua_newtable(L);
        lua_newtable(L);
        lua_setfield(L, -2, "pressed");
        lua_newtable(L);
        lua_setfield(L, -2, "released");
        lua_newtable(L);
        lua_setfield(L, -2, "state");
    lua_setglobal(L, "input");

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
    lua_setglobal(L, "buttons");

    lua_getglobal(L, "input");
        lua_getfield(L, -1, "pressed");
        input_state_table(L, buttons.pressed);
        lua_setfield(L, -2, "pressed");

        lua_getfield(L, -1, "released");
        input_state_table(L, buttons.released);
        lua_setfield(L, -2, "released");

        lua_getfield(L, -1, "state");
        input_state_table(L, buttons.state);
        lua_setfield(L, -2, "state");
    lua_setglobal(L, "input");

    *ud_joystick = joystick;
    *ud_tilt = tilt;
}