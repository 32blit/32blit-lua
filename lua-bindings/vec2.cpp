#include "../luablitlib.hpp"

using namespace blit;

static int vec2_dot(lua_State* L){
    int nargs = lua_gettop(L);
    Vec2 *vec2_a = reinterpret_cast<Vec2*>(lua_touserdata(L, 1));
    Vec2 *vec2_b = lua_blit_checkvec2(L, 2);
    float result = vec2_a->dot(*vec2_b);
    lua_pushnumber(L, result);
    return 1;
}

static int vec2_cross(lua_State* L){
    int nargs = lua_gettop(L);
    Vec2 *vec2_a = reinterpret_cast<Vec2*>(lua_touserdata(L, 1));
    Vec2 *vec2_b = lua_blit_checkvec2(L, 2);
    float result = vec2_a->cross(*vec2_b);
    lua_pushnumber(L, result);
    return 1;
}

static int vec2_angle(lua_State* L){
    int nargs = lua_gettop(L);
    Vec2 *vec2_a = reinterpret_cast<Vec2*>(lua_touserdata(L, 1));
    Vec2 *vec2_b = lua_blit_checkvec2(L, 2);
    float result = vec2_a->angle(*vec2_b);
    lua_pushnumber(L, result);
    return 1;
}

static int vec2_normalize(lua_State* L){
    int nargs = lua_gettop(L);
    Vec2 *vec2_a = reinterpret_cast<Vec2*>(lua_touserdata(L, 1));
    vec2_a->normalize();
    return 0;
}

static int vec2_rotate(lua_State* L){
    int nargs = lua_gettop(L);
    Vec2 *vec2_a = reinterpret_cast<Vec2*>(lua_touserdata(L, 1));
    float a = luaL_checknumber(L, 2);
    vec2_a->rotate(a);
    return 0;
}

static int vec2_new(lua_State* L){
    int nargs = lua_gettop(L);
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    lua_blit_pushvec2(L, Vec2(x, y));
    return 1;
}

static int vec2_delete(lua_State* L){
    reinterpret_cast<Vec2*>(lua_touserdata(L, 1))->~Vec2();
    lua_pop(L, 1);
    return 0;
}

static int vec2_index(lua_State* L){
    int nargs = lua_gettop(L);
    Vec2 *vec2 = reinterpret_cast<Vec2*>(lua_touserdata(L, 1));
    std::string_view method = luaL_checkstring(L, 2);
    if (nargs == 3) {
        if(method == "x") {
            vec2->x = luaL_checknumber(L, 3);
            return 0;
        }
        if(method == "y") {
            vec2->y = luaL_checknumber(L, 3);
            return 0;
        }
    }
    if(method == "x") {lua_pushnumber(L, vec2->x); return 1;}
    if(method == "y") {lua_pushnumber(L, vec2->y); return 1;}
    if(method == "length") {lua_pushnumber(L, vec2->length()); return 1;}  // TODO should this be a property or a method? This is probably faster.

    if(method == "cross") {lua_pushcfunction(L, vec2_cross); return 1;}
    if(method == "dot") {lua_pushcfunction(L, vec2_dot); return 1;}
    if(method == "angle") {lua_pushcfunction(L, vec2_angle); return 1;}
    if(method == "normalize") {lua_pushcfunction(L, vec2_normalize); return 1;}
    if(method == "rotate") {lua_pushcfunction(L, vec2_rotate); return 1;}

    luaL_error(L, "Index: Unknown property or method `%s` on %s", std::string(method).c_str(), LUA_BLIT_VEC2);
    return 0;
}

void lua_blit_pushvec2(lua_State* L, Vec2 p) {
    new(lua_newuserdata(L, sizeof(Vec2))) Vec2(p);
    luaL_setmetatable(L, LUA_BLIT_VEC2);
}

Vec2* lua_blit_checkvec2(lua_State *L, int arg) {
    auto vec2 = luaL_checkudata(L, arg, LUA_BLIT_VEC2);
    return reinterpret_cast<Vec2*>(vec2);
}

void lua_blit_register_vec2(lua_State *L) {
    lua_register(L, LUA_BLIT_VEC2, vec2_new);
    luaL_newmetatable(L, LUA_BLIT_VEC2);
    lua_pushcfunction(L, vec2_delete); lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, vec2_index); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, vec2_index); lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
}