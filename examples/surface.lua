source_rect = Rect(0, 0, 70, 14)
surface = nil
surface_test = nil
offset = nil
overlay = nil
overlay_rect = nil

function init()
    set_screen_mode(ScreenMode.hires)

    overlay_rect = Rect(Point(0, 0), screen.bounds)
    overlay = Surface(screen.bounds, PixelFormat.P)
    overlay.palette[0] = Pen(0, 0, 0, 0)
    overlay.palette[1] = Pen(255, 0, 0, 255)
    overlay.pen = Pen(0)
    overlay:clear()

    surface_test = Surface(source_rect.size)
    -- Test surface must be cleared with an opaque colour
    -- or blitting into it wont work- will just be transparent!
    surface_test.pen = Pen(0, 255, 0, 255)
    surface_test:clear()


    surface = Surface(source_rect.size)
    offset = overlay_rect.center - source_rect.center

    surface.pen = Pen(255, 255, 0)
    surface:clear()

    surface.pen = Pen(0, 0, 0)
    surface:text("Hello World", outline_font, source_rect.center, true, TextAlign.center_center)
end

function update(time)
end

function render(time)
    r = 50
    t = time / 1000
    ty = time / 2000
    tx = time / 500
    time_offset = Point(math.sin(t) * r, math.cos(t) * r) -- whoosh round and round
    time_offset.y = time_offset.y + (math.sin(ty) * 50)   -- up down wiggle
    time_offset.x = time_offset.x + (math.sin(tx) * 50)   -- left right wiggle
    dest_rect = Rect(
        offset + time_offset,
        Size(70 + (math.sin(tx) * 35), 14 + (math.sin(ty) * 7)))

    screen.pen = Pen(0, 0, 0)
    screen.clear()

    overlay.pen = Pen(1)
    overlay:pixel(offset + time_offset + source_rect.center)

    surface_test:blit(surface, source_rect, Point(0, 0))

    screen.blit(overlay, overlay_rect, Point(0, 0))
    screen.stretch_blit(surface_test, source_rect, dest_rect)
end
