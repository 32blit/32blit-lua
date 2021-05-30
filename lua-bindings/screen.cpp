#include "../luablitlib.hpp"

using namespace blit;

int sprite(lua_State *L) {
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

int load_sprites(lua_State *L) {
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

static int set_screen_mode(lua_State *L) {
    auto mode = static_cast<ScreenMode>(luaL_checkinteger(L, 1));
    blit::set_screen_mode(mode);
    return 0;
}

static int clear(lua_State *L) {
    screen.clear();
    return 0;
}

static int pen(lua_State *L) {
    Pen *p = lua_blit_checkpen(L, 1);
    screen.pen = *p;
    return 0;
}

static int bounds(lua_State *L) {
    lua_blit_pushsize(L, screen.bounds);
    return 1;
}

static int pixel(lua_State *L) {
    Point *p = lua_blit_checkpoint(L, 1);
    screen.pixel(*p);
    return 0;
}

static int h_span(lua_State *L) {
    Point *p = lua_blit_checkpoint(L, 1);
    auto c = luaL_checkinteger(L, 2);
    screen.h_span(*p, c);
    return 0;
}

static int v_span(lua_State *L) {
    Point *p = lua_blit_checkpoint(L, 1);
    auto c = luaL_checkinteger(L, 2);
    screen.v_span(*p, c);
    return 0;
}

static int line(lua_State *L) {
    Point *a = lua_blit_checkpoint(L, 1);
    Point *b = lua_blit_checkpoint(L, 2);
    screen.line(*a, *b);
    return 0;
}

static int rectangle(lua_State *L) {
    Rect *r = lua_blit_checkrect(L, 1);
    screen.rectangle(*r);
    return 0;
}

static int measure_text(lua_State *L) {
    int nargs = lua_gettop(L);
    size_t len;
    auto ptr = luaL_checklstring(L, 1, &len);
    auto message = std::string_view(ptr, len);
    auto variable = true;

    // TODO: handle invalid font
    blit::Font *font = (blit::Font *)lua_touserdata(L, 2);

    if(nargs >= 3) {
        variable = lua_toboolean(L, 3);
    }

    auto result = screen.measure_text(message, *font, variable);
    lua_blit_pushsize(L, result);
    return 1;
}

static int wrap_text(lua_State *L) {
    int nargs = lua_gettop(L);
    size_t len;
    auto ptr = luaL_checklstring(L, 1, &len);
    auto message = std::string_view(ptr, len);
    auto variable = true;

    // TODO: helper
    auto width = luaL_checkinteger(L, 2);

    // TODO: handle invalid font
    blit::Font *font = (blit::Font *)lua_touserdata(L, 3);

    if(nargs >= 4) {
        variable = lua_toboolean(L, 4);
    }

    std::string text = screen.wrap_text(message, width, *font, variable);
    lua_pushlstring(L, text.data(), text.length());
    return 1;
}

static int text(lua_State *L) {
    auto nargs = lua_gettop(L);
    size_t len;
    auto ptr = luaL_checklstring(L, 1, &len);
    auto message = std::string_view(ptr, len);
    auto variable = false;
    auto text_align = TextAlign::top_left;

    // TODO: handle invalid font
    blit::Font *font = (blit::Font *)lua_touserdata(L, 2);

    if(nargs >= 4) {
        variable = lua_toboolean(L, 4);
    }

    if(nargs >= 5) {
        text_align = static_cast<TextAlign>(luaL_checkinteger(L, 5));
    }

    // TODO: helper
    auto point = reinterpret_cast<Point *>(luaL_testudata(L, 3, LUA_BLIT_POINT));

    if(point) {
        screen.text(message, *font, *point, variable, text_align);
        return 0;
    }

    auto rect = reinterpret_cast<Rect *>(luaL_testudata(L, 3, LUA_BLIT_RECT));

    if(rect) {
        screen.text(message, *font, *rect, variable, text_align);
        return 0;
    }

    // error?

    return 0;
}


static int watermark(lua_State *L) {
    screen.watermark();
    return 0;
}

static int screen_index(lua_State* L){
    int nargs = lua_gettop(L);
    std::string_view method = luaL_checkstring(L, 2);

    if(nargs == 3) {
        if(method == "pen") {
            Pen *p = lua_blit_checkpen(L, 3);
            screen.pen = *p;
            return 0;
        }
        if(method == "clip") {
            Rect *r = lua_blit_checkrect(L, 3);
            screen.clip = *r;
            return 0;
        }
        if(method == "alpha") {
            screen.alpha = luaL_checkinteger(L, 3);
            return 0;
        }
    }

    if(method == "test") {lua_pushinteger(L, 59); return 1;}
    if(method == "bounds") {lua_blit_pushsize(L, screen.bounds); return 1;}
    if(method == "pen") {lua_blit_pushpen(L, screen.pen); return 1;}
    if(method == "alpha") {lua_pushinteger(L, screen.alpha); return 1;}

    luaL_error(L, "Unknown property or method `%s` on %s", std::string(method).c_str(), "screen");
    return 0;
}

static const luaL_Reg screen_funcs[] = {
    {"pixel", pixel},
    {"h_span", h_span},
    {"v_span", v_span},
    {"line", line},
    {"rectangle", rectangle},
    {"text", text},
    {"wrap_text", wrap_text},
    {"measure_text", measure_text},
    {"clear", clear},
    {"watermark", watermark},
    {"load_sprites", load_sprites},
    {"sprite", sprite},
    {NULL, NULL}
};

static const luaL_Reg screen_meta_funcs[] = {
    {"__index", screen_index},
    {"__newindex", screen_index},
    {NULL, NULL}
};

void lua_blit_setup_screen (lua_State *L) {
    luaL_newmetatable(L, "screen");
    lua_pushvalue(L, -1);
    luaL_setfuncs(L, screen_funcs, 0);
    luaL_newlib(L, screen_meta_funcs);
    lua_setmetatable(L, -2);
    lua_setglobal(L, "screen");

    lua_pushcfunction(L, set_screen_mode);
    lua_setglobal(L, "set_screen_mode");
}