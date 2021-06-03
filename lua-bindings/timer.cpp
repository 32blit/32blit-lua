#include "../luablitlib.hpp"

using namespace blit;

static int timer_start(lua_State* L){
    reinterpret_cast<Timer*>(lua_touserdata(L, 1))->start();
    return 0;
}

static int timer_init(lua_State* L){
    int nargs = lua_gettop(L);
    Timer *timer = reinterpret_cast<Timer*>(lua_touserdata(L, 1));

    if(lua_type(L, 2) != LUA_TFUNCTION) {
        return 0; // TODO throw some kind of exception here
    }

    lua_Integer loop_count = -1;
    if(nargs >= 4) {
        luaL_checkinteger(L, 4);
        lua_pop(L, 1);
    }

    lua_Integer timeout = luaL_checkinteger(L, 3);
    lua_pop(L, 1);

    lua_Integer callback = luaL_ref(L, LUA_REGISTRYINDEX);
    timer->init([callback, L](Timer &t) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, callback);
        if(lua_pcall(L, 0, 0, 0 ) != 0){
            printf("Failed to call the callback!\n %s\n", lua_tostring(L, -1));
            return;
        }
    }, timeout, loop_count);

    return 0;
}

static int timer_new(lua_State* L){
    new(lua_newuserdata(L, sizeof(Timer))) Timer;
    luaL_setmetatable(L, LUA_BLIT_TIMER);
    return 1;
}

static int timer_delete(lua_State* L){
    Timer *timer = reinterpret_cast<Timer*>(lua_touserdata(L, 1));
    delete timer;
    return 0;
}

static int timer_index(lua_State* L){
    Timer *timer = reinterpret_cast<Timer*>(lua_touserdata(L, 1));
    std::string_view method = luaL_checkstring(L, 2);

    if(method == "init") {lua_pushcfunction(L, timer_init); return 1;}
    if(method == "start") {lua_pushcfunction(L, timer_start); return 1;}

    luaL_error(L, "Unknown property or method `%s` on %s", method.data(), LUA_BLIT_PEN);
    return 0;
}

Timer* lua_blit_checktimer(lua_State *L, int arg) {
    auto t = luaL_checkudata(L, arg, LUA_BLIT_TIMER);
    return reinterpret_cast<Timer*>(t);
}

void lua_blit_register_timer(lua_State *L) {
    lua_register(L, LUA_BLIT_TIMER, timer_new);
    luaL_newmetatable(L, LUA_BLIT_TIMER);
    lua_pushcfunction(L, timer_delete); lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, timer_index); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, timer_index); lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
}