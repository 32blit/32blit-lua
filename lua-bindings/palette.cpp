#include "../luablitlib.hpp"

using namespace blit;

static int palette_new(lua_State* L){
    new(lua_newuserdata(L, sizeof(Palette))) Palette{new Pen[256]};
    luaL_setmetatable(L, LUA_BLIT_PALETTE);
    return 1;
}

static int palette_delete(lua_State* L){
    //auto palette = reinterpret_cast<Palette*>(lua_touserdata(L, 1));
    return 0;
}

static int palette_index(lua_State* L){
    int nargs = lua_gettop(L);
    auto palette = reinterpret_cast<Palette*>(lua_touserdata(L, 1));
    if(lua_isinteger(L, 2)) {
        auto index = lua_tointeger(L, 2);
        if(index >= 0 && index <= 255) {
            if(nargs == 3) {
                auto pen = lua_blit_checkpen(L, 3);
                palette->entries[index] = *pen;
                return 0;
            } else {
                lua_blit_pushpen(L, palette->entries[index]); return 1;
            }
        }
        luaL_error(L, "Index `%d` out of range on %s", index, LUA_BLIT_PALETTE);
    } else {
        std::string_view method = luaL_checkstring(L, 2);
        luaL_error(L, "Unknown property or method `%s` on %s", method.data(), LUA_BLIT_PALETTE);
    }
    return 0;
}

void lua_blit_pushpalette(lua_State *L, Pen *p) {
    new(lua_newuserdata(L, sizeof(Palette))) Palette{p};
    luaL_setmetatable(L, LUA_BLIT_PALETTE);
}

void lua_blit_pushpalette(lua_State *L, Palette *p) {
    new(lua_newuserdata(L, sizeof(Palette))) Palette{p->entries};
    luaL_setmetatable(L, LUA_BLIT_PALETTE);
}

Palette* lua_blit_checkpalette(lua_State *L, int arg) {
    auto p = luaL_checkudata(L, arg, LUA_BLIT_PALETTE);
    return reinterpret_cast<Palette*>(p);
}

void lua_blit_register_palette(lua_State *L) {
    lua_register(L, LUA_BLIT_PALETTE, palette_new);
    luaL_newmetatable(L, LUA_BLIT_PALETTE);
    lua_pushcfunction(L, palette_delete); lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, palette_index); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, palette_index); lua_setfield(L, -2, "__newindex");
}