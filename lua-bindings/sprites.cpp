#include "../luablitlib.hpp"

using namespace blit;

static int sprite(lua_State *L) {
    int nargs = lua_gettop(L);
    unsigned int i = luaL_checknumber(L, 1);
    Point *p = lua_blit_checkpoint(L, 2);
    lua_pop(L, nargs);
    screen.sprite(i, *p);
    return 0;
}

static int load_sprites(lua_State *L) {
    int nargs = lua_gettop(L);
    std::string filename = luaL_checkstring(L, 1);
    lua_pop(L, nargs);
    if(screen.sprites != nullptr) {
        delete screen.sprites->data;
        delete screen.sprites->palette;
        delete screen.sprites;
        screen.sprites = nullptr;
    }
    screen.sprites = SpriteSheet::load(filename);
    return 0;
}

static const luaL_Reg funcs[] = {
    {"load_sprites", load_sprites},
    {"sprite", sprite},
    {NULL, NULL}
};

void lua_blit_register_sprites(lua_State *L) {
    luaL_setfuncs(L, funcs, 0);
}