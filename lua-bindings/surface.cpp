#include "../luablitlib.hpp"

using namespace blit;

static int surface_clear(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    surface->clear();
    return 0;
}

static int surface_pixel(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    Point *p = lua_blit_checkpoint(L, 2);
    surface->pixel(*p);
    return 0;
}

static int surface_h_span(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    Point *p = lua_blit_checkpoint(L, 2);
    auto c = luaL_checkinteger(L, 3);
    surface->h_span(*p, c);
    return 0;
}

static int surface_v_span(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    Point *p = lua_blit_checkpoint(L, 2);
    auto c = luaL_checkinteger(L, 3);
    surface->v_span(*p, c);
    return 0;
}

static int surface_line(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    Point *a = lua_blit_checkpoint(L, 2);
    Point *b = lua_blit_checkpoint(L, 3);
    surface->line(*a, *b);
    return 0;
}

static int surface_rectangle(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    Rect *r = lua_blit_checkrect(L, 2);
    surface->rectangle(*r);
    return 0;
}

static int surface_circle(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    Point *c = lua_blit_checkpoint(L, 2);
    int32_t r = luaL_checkinteger(L, 3);
    surface->circle(*c, r);
    return 0;
}

static int surface_polygon(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    int count = lua_gettop(L);
    std::vector <Point> points;
    if(count == 2 && lua_type(L, 2) == LUA_TTABLE)
    {
        // TODO this might be fragile, but how much type checking do we really want?
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            if(lua_type(L, -1) == LUA_TUSERDATA) {
                points.push_back(*lua_blit_checkpoint(L, -1));
            }
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
        surface->polygon(points);
    }
    else
    {
        for(auto p = 0; p < count; p++) {
            points.push_back(*lua_blit_checkpoint(L, p + 2));
        }
        surface->polygon(points);
    }
    return 0;
}

static int surface_measure_text(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    int nargs = lua_gettop(L);
    size_t len;
    auto ptr = luaL_checklstring(L, 2, &len);
    auto message = std::string_view(ptr, len);
    auto variable = true;

    // TODO: handle invalid font
    blit::Font *font = (blit::Font *)lua_touserdata(L, 3);

    if(nargs >= 4) {
        variable = lua_toboolean(L, 4);
    }

    auto result = surface->measure_text(message, *font, variable);
    lua_blit_pushsize(L, result);
    return 1;
}

static int surface_wrap_text(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    int nargs = lua_gettop(L);
    size_t len;
    auto ptr = luaL_checklstring(L, 2, &len);
    auto message = std::string_view(ptr, len);
    auto variable = true;

    // TODO: helper
    auto width = luaL_checkinteger(L, 3);

    // TODO: handle invalid font
    blit::Font *font = (blit::Font *)lua_touserdata(L, 4);

    if(nargs >= 5) {
        variable = lua_toboolean(L, 5);
    }

    std::string text = surface->wrap_text(message, width, *font, variable);
    lua_pushlstring(L, text.data(), text.length());
    return 1;
}

static int surface_text(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    auto nargs = lua_gettop(L);
    size_t len;
    auto ptr = luaL_checklstring(L, 2, &len);
    auto message = std::string_view(ptr, len);
    auto variable = false;
    auto text_align = TextAlign::top_left;

    // TODO: handle invalid font
    blit::Font *font = (blit::Font *)lua_touserdata(L, 3);

    if(nargs >= 5) {
        variable = lua_toboolean(L, 5);
    }

    if(nargs >= 6) {
        text_align = static_cast<TextAlign>(luaL_checkinteger(L, 6));
    }

    // TODO: helper
    auto point = reinterpret_cast<Point *>(luaL_testudata(L, 4, LUA_BLIT_POINT));

    if(point) {
        surface->text(message, *font, *point, variable, text_align);
        return 0;
    }

    auto rect = reinterpret_cast<Rect *>(luaL_testudata(L, 4, LUA_BLIT_RECT));

    if(rect) {
        surface->text(message, *font, *rect, variable, text_align);
        return 0;
    }

    // error?

    return 0;
}

static int surface_sprite(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    Point *pos = lua_blit_checkpoint(L, 3);

    // (int, point)
    if(lua_isinteger(L, 2)) {
        unsigned int i = lua_tointeger(L, 2);
        surface->sprite(i, *pos);
        return 0;
    }

    // (Point, Point)
    auto p = reinterpret_cast<Point *>(luaL_testudata(L, 2, LUA_BLIT_POINT));
    if(p) {
        surface->sprite(*p, *pos);
        return 0;
    }

    // (Rect, Point)
    auto r = reinterpret_cast<Rect *>(luaL_testudata(L, 2, LUA_BLIT_RECT));
    if(r) {
        surface->sprite(*r, *pos);
        return 0;
    }

    //error?

    return 0;
}

static int surface_load_sprites(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    std::string filename = luaL_checkstring(L, 2);
    if(surface->sprites != nullptr) {
        delete surface->sprites->data;
        delete surface->sprites->palette;
        delete surface->sprites;
        surface->sprites = nullptr;
    }
    surface->sprites = Surface::load(filename);
    return 0;
}

static int surface_blit(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    int nargs = lua_gettop(L);
    bool hflip = false;
    if(nargs >= 5) {
        hflip = lua_toboolean(L, 5);
    }
    auto source_surface = lua_blit_checksurface(L, 2);
    auto source_rect = lua_blit_checkrect(L, 3);
    auto dest_point = lua_blit_checkpoint(L, 4);
    surface->blit(source_surface, *source_rect, *dest_point, hflip);
    return 0;
}

static int surface_stretch_blit(lua_State *L) {
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    auto source_surface = lua_blit_checksurface(L, 2);
    auto source_rect = lua_blit_checkrect(L, 3);
    auto dest_rect = lua_blit_checkrect(L, 4);
    surface->stretch_blit(source_surface, *source_rect, *dest_rect);
    return 0;
}

static int surface_new(lua_State* L){
    int nargs = lua_gettop(L);
    auto pixel_format = PixelFormat::RGBA;
    auto bounds = lua_blit_checksize(L, 1);

    if(nargs >= 2) {
        pixel_format = static_cast<PixelFormat>(luaL_checkinteger(L, 2));
    }

    uint8_t *data = new uint8_t[bounds->w * bounds->h * pixel_format_stride[static_cast<uint8_t>(pixel_format)]];
    new(lua_newuserdata(L, sizeof(Surface))) Surface(data, pixel_format, *bounds);
    luaL_setmetatable(L, LUA_BLIT_SURFACE);
    return 1;
}

static int surface_delete(lua_State* L){
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    if(surface->sprites != nullptr) {
        delete surface->sprites->data;
        delete surface->sprites->palette;
        delete surface->sprites;
        surface->sprites = nullptr;
    }
    delete surface->data;
    delete surface;
    return 0;
}

static int surface_index(lua_State* L){
    int nargs = lua_gettop(L);
    Surface *surface = reinterpret_cast<Surface*>(lua_touserdata(L, 1));
    std::string_view method = luaL_checkstring(L, 2);

    if(method == "pixel") {lua_pushcfunction(L, surface_pixel); return 1;}
    if(method == "h_span") {lua_pushcfunction(L, surface_h_span); return 1;}
    if(method == "v_span") {lua_pushcfunction(L, surface_v_span); return 1;}
    if(method == "line") {lua_pushcfunction(L, surface_line); return 1;}
    if(method == "rectangle") {lua_pushcfunction(L, surface_rectangle); return 1;}
    if(method == "circle") {lua_pushcfunction(L, surface_circle); return 1;}
    if(method == "polygon") {lua_pushcfunction(L, surface_polygon); return 1;}
    if(method == "text") {lua_pushcfunction(L, surface_text); return 1;}
    if(method == "wrap_text") {lua_pushcfunction(L, surface_wrap_text); return 1;}
    if(method == "measure_text") {lua_pushcfunction(L, surface_measure_text); return 1;}
    if(method == "clear") {lua_pushcfunction(L, surface_clear); return 1;}
    if(method == "load_sprites") {lua_pushcfunction(L, surface_load_sprites); return 1;}
    if(method == "sprite") {lua_pushcfunction(L, surface_sprite); return 1;}
    if(method == "blit") {lua_pushcfunction(L, surface_blit); return 1;}
    if(method == "stretch_blit") {lua_pushcfunction(L, surface_stretch_blit); return 1;}

    if(nargs == 3) {
        if(method == "pen") {
            Pen *p = lua_blit_checkpen(L, 3);
            surface->pen = *p;
            return 0;
        }
        if(method == "clip") {
            Rect *r = lua_blit_checkrect(L, 3);
            surface->clip = *r;
            return 0;
        }
        if(method == "alpha") {
            surface->alpha = luaL_checkinteger(L, 3);
            return 0;
        }
        if(surface->format == PixelFormat::P && method == "palette") {
            auto palette = lua_blit_checkpalette(L, 3);
            surface->palette = palette->entries;
            return 0;
        }
    } else {
        if(method == "bounds") {lua_blit_pushsize(L, surface->bounds); return 1;}
        if(method == "pen") {lua_blit_pushpen(L, surface->pen); return 1;}
        if(method == "alpha") {lua_pushinteger(L, surface->alpha); return 1;}
        if(surface->format == PixelFormat::P && method == "palette") {
            if(!surface->palette) {
                surface->palette = new Pen[256];
            }
            lua_blit_pushpalette(L, surface->palette);
            return 1;
        }
    }

    luaL_error(L, "Unknown property or method `%s` on %s", method.data(), LUA_BLIT_SURFACE);
    return 0;
}

void lua_blit_pushsurface(lua_State* L, Surface s) {
    new(lua_newuserdata(L, sizeof(Surface))) Surface(s);
    luaL_setmetatable(L, LUA_BLIT_SURFACE);
}

Surface* lua_blit_checksurface(lua_State *L, int arg) {
    auto t = luaL_checkudata(L, arg, LUA_BLIT_SURFACE);
    return reinterpret_cast<Surface*>(t);
}

void lua_blit_register_surface(lua_State *L) {
    lua_register(L, LUA_BLIT_SURFACE, surface_new);
    luaL_newmetatable(L, LUA_BLIT_SURFACE);
    lua_pushcfunction(L, surface_delete); lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, surface_index); lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, surface_index); lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);
}