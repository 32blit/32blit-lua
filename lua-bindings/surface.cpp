#include "../luablitlib.hpp"

using namespace blit;

static Surface *to_surface(lua_State *L, int arg) {
    return reinterpret_cast<Surface*>(lua_touserdata(L, arg));
}

// shared between Surface and screen
static int surface_clear(lua_State *L, Surface *surface, int first_arg) {
    surface->clear();
    return 0;
}

static int surface_pixel(lua_State *L, Surface *surface, int first_arg) {
    Point *p = lua_blit_checkpoint(L, first_arg);
    surface->pixel(*p);
    return 0;
}

static int surface_h_span(lua_State *L, Surface *surface, int first_arg) {
    Point *p = lua_blit_checkpoint(L, first_arg);
    auto c = luaL_checkinteger(L, first_arg + 1);
    surface->h_span(*p, c);
    return 0;
}

static int surface_v_span(lua_State *L, Surface *surface, int first_arg) {
    Point *p = lua_blit_checkpoint(L, first_arg);
    auto c = luaL_checkinteger(L, first_arg + 1);
    surface->v_span(*p, c);
    return 0;
}

static int surface_line(lua_State *L, Surface *surface, int first_arg) {
    Point *a = lua_blit_checkpoint(L, first_arg);
    Point *b = lua_blit_checkpoint(L, first_arg + 1);
    surface->line(*a, *b);
    return 0;
}

static int surface_rectangle(lua_State *L, Surface *surface, int first_arg) {
    Rect *r = lua_blit_checkrect(L, first_arg);
    surface->rectangle(*r);
    return 0;
}

static int surface_circle(lua_State *L, Surface *surface, int first_arg) {
    Point *c = lua_blit_checkpoint(L, first_arg);
    int32_t r = luaL_checkinteger(L, first_arg + 1);
    surface->circle(*c, r);
    return 0;
}

static int surface_polygon(lua_State *L, Surface *surface, int first_arg) {
    int count = lua_gettop(L);
    std::vector <Point> points;
    if(count == first_arg && lua_type(L, first_arg) == LUA_TTABLE)
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
            points.push_back(*lua_blit_checkpoint(L, p + first_arg));
        }
        surface->polygon(points);
    }
    return 0;
}

static int surface_measure_text(lua_State *L, Surface *surface, int first_arg) {
    int nargs = lua_gettop(L);
    size_t len;
    auto ptr = luaL_checklstring(L, first_arg, &len);
    auto message = std::string_view(ptr, len);
    auto variable = true;

    // TODO: handle invalid font
    blit::Font *font = (blit::Font *)lua_touserdata(L, first_arg + 1);

    if(nargs >= first_arg + 2) {
        variable = lua_toboolean(L, first_arg + 2);
    }

    auto result = surface->measure_text(message, *font, variable);
    lua_blit_pushsize(L, result);
    return 1;
}

static int surface_wrap_text(lua_State *L, Surface *surface, int first_arg) {
    int nargs = lua_gettop(L);
    size_t len;
    auto ptr = luaL_checklstring(L, first_arg, &len);
    auto message = std::string_view(ptr, len);
    auto variable = true;

    // TODO: helper
    auto width = luaL_checkinteger(L, first_arg + 1);

    // TODO: handle invalid font
    blit::Font *font = (blit::Font *)lua_touserdata(L, first_arg + 2);

    if(nargs >= first_arg + 3) {
        variable = lua_toboolean(L, first_arg + 3);
    }

    std::string text = surface->wrap_text(message, width, *font, variable);
    lua_pushlstring(L, text.data(), text.length());
    return 1;
}

static int surface_text(lua_State *L, Surface *surface, int first_arg) {
    auto nargs = lua_gettop(L);
    size_t len;
    auto ptr = luaL_checklstring(L, first_arg, &len);
    auto message = std::string_view(ptr, len);
    auto variable = false;
    auto text_align = TextAlign::top_left;

    // TODO: handle invalid font
    blit::Font *font = (blit::Font *)lua_touserdata(L, first_arg + 1);

    if(nargs >= first_arg + 3) {
        variable = lua_toboolean(L, first_arg + 3);
    }

    if(nargs >= first_arg + 4) {
        text_align = static_cast<TextAlign>(luaL_checkinteger(L, first_arg + 4));
    }

    // TODO: helper
    auto point = reinterpret_cast<Point *>(luaL_testudata(L, first_arg + 2, LUA_BLIT_POINT));

    if(point) {
        surface->text(message, *font, *point, variable, text_align);
        return 0;
    }

    auto rect = reinterpret_cast<Rect *>(luaL_testudata(L, first_arg + 2, LUA_BLIT_RECT));

    if(rect) {
        surface->text(message, *font, *rect, variable, text_align);
        return 0;
    }

    // error?

    return 0;
}

static int surface_sprite(lua_State *L, Surface *surface, int first_arg) {
    auto nargs = lua_gettop(L);
    Point *pos = lua_blit_checkpoint(L, first_arg + 1);

    Point origin(0, 0);
    Vec2 scale(1.0f, 1.0f);
    uint8_t transform = 0;
    bool has_origin = false;

    // transform is always last
    int transform_arg = first_arg + 2;

    // check for origin
    if(nargs >= transform_arg) {
        auto p = reinterpret_cast<Point *>(luaL_testudata(L, transform_arg, LUA_BLIT_POINT));
        if(p) {
            // origin
            origin = *p;
            transform_arg++;
            has_origin = true;
        }
        // else if !int error
    }

    // check for scale
    if(has_origin && nargs >= transform_arg) {
        auto v = reinterpret_cast<Vec2 *>(luaL_testudata(L, transform_arg, LUA_BLIT_VEC2));
        if(v) {
            // scale
            scale = *v;
            transform_arg++;
        } else if(lua_isnumber(L, transform_arg)) {
            float f = lua_tonumber(L, transform_arg);
            scale = Vec2(f, f);
            transform_arg++;
        }
        // else if !int error
    }

    // ... and the transform
    if(nargs >= transform_arg && lua_isinteger(L, transform_arg)) {
        transform = lua_tointeger(L, transform_arg);
    }

    // (int, point)
    if(lua_isinteger(L, first_arg)) {
        unsigned int i = lua_tointeger(L, first_arg);
        // TODO: always using the scale overload will be slower (blit vs stretch_blit)
        surface->sprite(i, *pos, origin, scale, transform);
        return 0;
    }

    // (Point, Point)
    auto p = reinterpret_cast<Point *>(luaL_testudata(L, first_arg, LUA_BLIT_POINT));
    if(p) {
        surface->sprite(*p, *pos, origin, scale, transform);
        return 0;
    }

    // (Rect, Point)
    auto r = reinterpret_cast<Rect *>(luaL_testudata(L, first_arg, LUA_BLIT_RECT));
    if(r) {
        surface->sprite(*r, *pos, origin, scale, transform);
        return 0;
    }

    //error?

    return 0;
}

static int surface_load_sprites(lua_State *L, Surface *surface, int first_arg) {
    std::string filename = luaL_checkstring(L, first_arg);
    if(surface->sprites != nullptr) {
        delete surface->sprites->data;
        delete surface->sprites->palette;
        delete surface->sprites;
        surface->sprites = nullptr;
    }
    surface->sprites = Surface::load(filename);
    return 0;
}

static int surface_blit(lua_State *L, Surface *surface, int first_arg) {
    int nargs = lua_gettop(L);
    auto source_surface = lua_blit_checksurface(L, first_arg);
    auto source_rect = lua_blit_checkrect(L, first_arg + 1);
    auto dest_point = lua_blit_checkpoint(L, first_arg + 2);

    if(nargs >= first_arg + 3) {
        auto transforms = lua_tointeger(L, first_arg + 3);
        surface->blit(source_surface, *source_rect, *dest_point, transforms);
    } else
        surface->blit(source_surface, *source_rect, *dest_point);
    return 0;
}

static int surface_stretch_blit(lua_State *L, Surface *surface, int first_arg) {
    int nargs = lua_gettop(L);
    auto source_surface = lua_blit_checksurface(L, first_arg);
    auto source_rect = lua_blit_checkrect(L, first_arg + 1);
    auto dest_rect = lua_blit_checkrect(L, first_arg + 2);

    if(nargs >= first_arg + 3) {
        auto transforms = lua_tointeger(L, first_arg + 3);
        surface->stretch_blit(source_surface, *source_rect, *dest_rect, transforms);
    } else
        surface->stretch_blit(source_surface, *source_rect, *dest_rect);
    return 0;
}

static int surface_watermark(lua_State *L, Surface *surface, int first_arg) {
    screen.watermark();
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

static int surface_index(lua_State *L, Surface *surface, int nargs, std::string_view method) {
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

// surface wrappers
static int surface_pixel(lua_State *L) {
    return surface_pixel(L, to_surface(L, 1), 2);
}

static int surface_h_span(lua_State *L) {
    return surface_h_span(L, to_surface(L, 1), 2);
}

static int surface_v_span(lua_State *L) {
    return surface_v_span(L, to_surface(L, 1), 2);
}

static int surface_line(lua_State *L) {
    return surface_line(L, to_surface(L, 1), 2);
}

static int surface_rectangle(lua_State *L) {
    return surface_rectangle(L, to_surface(L, 1), 2);
}

static int surface_circle(lua_State *L) {
    return surface_circle(L, to_surface(L, 1), 2);
}

static int surface_polygon(lua_State *L) {
    return surface_polygon(L, to_surface(L, 1), 2);
}

static int surface_text(lua_State *L) {
    return surface_text(L, to_surface(L, 1), 2);
}

static int surface_wrap_text(lua_State *L) {
    return surface_wrap_text(L, to_surface(L, 1), 2);
}

static int surface_measure_text(lua_State *L) {
    return surface_measure_text(L, to_surface(L, 1), 2);
}

static int surface_clear(lua_State *L) {
    return surface_clear(L, to_surface(L, 1), 2);
}

static int surface_watermark(lua_State *L) {
    return surface_watermark(L, to_surface(L, 1), 2);
}

static int surface_load_sprites(lua_State *L) {
    return surface_load_sprites(L, to_surface(L, 1), 2);
}

static int surface_sprite(lua_State *L) {
    return surface_sprite(L, to_surface(L, 1), 2);
}

static int surface_blit(lua_State *L) {
    return surface_blit(L, to_surface(L, 1), 2);
}

static int surface_stretch_blit(lua_State *L) {
    return surface_stretch_blit(L, to_surface(L, 1), 2);
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

    return surface_index(L, surface, nargs, method);
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

// screen
static int set_screen_mode(lua_State *L) {
    auto mode = static_cast<ScreenMode>(luaL_checkinteger(L, 1));
    blit::set_screen_mode(mode);
    return 0;
}

// wrappers
static int screen_pixel(lua_State *L) {
    return surface_pixel(L, &screen, 1);
}

static int screen_h_span(lua_State *L) {
    return surface_h_span(L, &screen, 1);
}

static int screen_v_span(lua_State *L) {
    return surface_v_span(L, &screen, 1);
}

static int screen_line(lua_State *L) {
    return surface_line(L, &screen, 1);
}

static int screen_rectangle(lua_State *L) {
    return surface_rectangle(L, &screen, 1);
}

static int screen_circle(lua_State *L) {
    return surface_circle(L, &screen, 1);
}

static int screen_polygon(lua_State *L) {
    return surface_polygon(L, &screen, 1);
}

static int screen_text(lua_State *L) {
    return surface_text(L, &screen, 1);
}

static int screen_wrap_text(lua_State *L) {
    return surface_wrap_text(L, &screen, 1);
}

static int screen_measure_text(lua_State *L) {
    return surface_measure_text(L, &screen, 1);
}

static int screen_clear(lua_State *L) {
    return surface_clear(L, &screen, 1);
}

static int screen_watermark(lua_State *L) {
    return surface_watermark(L, &screen, 1);
}

static int screen_load_sprites(lua_State *L) {
    return surface_load_sprites(L, &screen, 1);
}

static int screen_sprite(lua_State *L) {
    return surface_sprite(L, &screen, 1);
}

static int screen_blit(lua_State *L) {
    return surface_blit(L, &screen, 1);
}

static int screen_stretch_blit(lua_State *L) {
    return surface_stretch_blit(L, &screen, 1);
}


static int screen_index(lua_State* L){
    int nargs = lua_gettop(L);
    std::string_view method = luaL_checkstring(L, 2);

    return surface_index(L, &screen, nargs, method);
}

static const luaL_Reg screen_funcs[] = {
    {"pixel", screen_pixel},
    {"h_span", screen_h_span},
    {"v_span", screen_v_span},
    {"line", screen_line},
    {"rectangle", screen_rectangle},
    {"circle", screen_circle},
    {"polygon", screen_polygon},
    {"text", screen_text},
    {"wrap_text", screen_wrap_text},
    {"measure_text", screen_measure_text},
    {"clear", screen_clear},
    {"watermark", screen_watermark},
    {"load_sprites", screen_load_sprites},
    {"sprite", screen_sprite},
    {"blit", screen_blit},
    {"stretch_blit", screen_stretch_blit},
    {NULL, NULL}
};

static const luaL_Reg screen_meta_funcs[] = {
    {"__index", screen_index},
    {"__newindex", screen_index},
    {NULL, NULL}
};

void lua_blit_setup_screen (lua_State *L) {
    // TODO try to make this an instance of the Surface bindings
    // Assuming that doesn't totally tank performance!
    luaL_newmetatable(L, "screen");
    lua_pushvalue(L, -1);
    luaL_setfuncs(L, screen_funcs, 0);
    luaL_newlib(L, screen_meta_funcs);
    lua_setmetatable(L, -2);
    lua_setglobal(L, "screen");

    lua_pushcfunction(L, set_screen_mode);
    lua_setglobal(L, "set_screen_mode");
}
