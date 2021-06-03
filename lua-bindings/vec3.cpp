#include "../luablitlib.hpp"

using namespace blit;

static int vec3_dot(lua_State* L){
    Vec3 *vec3_a = reinterpret_cast<Vec3*>(lua_touserdata(L, 1));
    Vec3 *vec3_b = lua_blit_checkvec3(L, 2);
    float result = vec3_a->dot(*vec3_b);
    lua_pushnumber(L, result);
    return 1;
}

static int vec3_cross(lua_State* L){
    Vec3 *vec3_a = reinterpret_cast<Vec3*>(lua_touserdata(L, 1));
    Vec3 *vec3_b = lua_blit_checkvec3(L, 2);
    Vec3 result = vec3_a->cross(*vec3_b);
    lua_blit_pushvec3(L, result);
    return 1;
}

static int vec3_new(lua_State* L){
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    lua_blit_pushvec3(L, Vec3(x, y));
    return 1;
}

static int vec3_delete(lua_State* L){
    reinterpret_cast<Vec3*>(lua_touserdata(L, 1))->~Vec3();
    lua_pop(L, 1);
    return 0;
}

static int vec3_normalize(lua_State* L){
    Vec3 *vec3_a = reinterpret_cast<Vec3*>(lua_touserdata(L, 1));
    vec3_a->normalize();
    return 0;
}

static int vec3_index(lua_State* L){
    int nargs = lua_gettop(L);
    Vec3 *vec3 = reinterpret_cast<Vec3*>(lua_touserdata(L, 1));
    std::string_view method = luaL_checkstring(L, 2);
    if (nargs == 3) {
        if(method == "x") {
            vec3->x = luaL_checknumber(L, 3);
            return 0;
        }
        if(method == "y") {
            vec3->y = luaL_checknumber(L, 3);
            return 0;
        }
        if(method == "z") {
            vec3->z = luaL_checknumber(L, 3);
            return 0;
        }
    }
    if(method == "x") {lua_pushnumber(L, vec3->x); return 1;}
    if(method == "y") {lua_pushnumber(L, vec3->y); return 1;}
    if(method == "z") {lua_pushnumber(L, vec3->z); return 1;}
    if(method == "length") {lua_pushnumber(L, vec3->length()); return 1;}  // TODO should this be a property or a method? This is probably faster.

    if(method == "cross") {lua_pushcfunction(L, vec3_cross); return 1;}
    if(method == "dot") {lua_pushcfunction(L, vec3_dot); return 1;}
    if(method == "normalize") {lua_pushcfunction(L, vec3_normalize); return 1;}

    luaL_error(L, "Index: Unknown property or method `%s` on %s", std::string(method).c_str(), LUA_BLIT_VEC2);
    return 0;
}

void lua_blit_pushvec3(lua_State* L, Vec3 p) {
    new(lua_newuserdata(L, sizeof(Vec3))) Vec3(p);
    luaL_setmetatable(L, LUA_BLIT_VEC3);
}

Vec3* lua_blit_checkvec3(lua_State *L, int arg) {
    auto vec3 = luaL_checkudata(L, arg, LUA_BLIT_VEC3);
    return reinterpret_cast<Vec3*>(vec3);
}

void lua_blit_register_vec3(lua_State *L) {
    lua_register(L, LUA_BLIT_VEC3, vec3_new);
    luaL_newmetatable(L, LUA_BLIT_VEC3);
    lua_pushcfunction(L, vec3_delete); lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, vec3_index); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, vec3_index); lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
}