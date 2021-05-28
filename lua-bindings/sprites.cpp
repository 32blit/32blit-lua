#include "../luablitlib.hpp"

using namespace blit;

static int sprite(lua_State *L) {
    Point *pos = lua_blit_checkpoint(L, 2);

    // (int, point)
    if(lua_isinteger(L, 1)) {
        unsigned int i = lua_tointeger(L, 1);
        screen.sprite(i, *pos);
        return 0;
    }

    // (Point, Point)
    auto p = reinterpret_cast<Point *>(luaL_testudata(L, 1, LUA_BLIT_POINT));
    if(p) {
        screen.sprite(*p, *pos);
        return 0;
    }

    // (Rect, Point)
    auto r = reinterpret_cast<Rect *>(luaL_testudata(L, 1, LUA_BLIT_RECT));
    if(r) {
        screen.sprite(*r, *pos);
        return 0;
    }

    //error?

    return 0;
}

static int load_sprites(lua_State *L) {
    int nargs = lua_gettop(L);
    std::string filename = luaL_checkstring(L, 1);
    if(screen.sprites != nullptr) {
        delete screen.sprites->data;
        delete screen.sprites->palette;
        delete screen.sprites;
        screen.sprites = nullptr;
    }
    screen.sprites = Surface::load(filename);
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