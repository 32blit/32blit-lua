surface = nil
surface_rect = Rect(0, 0, 50, 50)

function init()
    set_screen_mode(ScreenMode.hires)

    surface = Surface(surface_rect.size)
end

function update(time)
end

function regular_polygon(polygon_center, polygon_sides, polygon_radius, polygon_rotation)
    local regular_polygon = {}
    polygon_rotation = polygon_rotation or 0
    polygon_rotation = math.rad(polygon_rotation % 360)

    -- The angle between each side
    polygon_angle = (math.pi * 2) / polygon_sides

    for i = 0, polygon_sides, 1 do
        angle = polygon_angle * i
        angle = angle + polygon_rotation

        x = math.sin(angle) * polygon_radius
        y = -math.cos(angle) * polygon_radius

        -- Translate the polygon to the desired origin
        -- Since sin/cos produce values from -1 to +1 its origin is 0, 0
        x = x + polygon_center.x
        y = y + polygon_center.y
        regular_polygon[#regular_polygon + 1] = Point(x, y)
    end

    return regular_polygon
end

function render(time)
    screen.pen = Pen(0, 0, 0)
    screen.clear()

    surface.pen = Pen(0, 0, 0)
    surface:clear()

    -- Create a 120x120 pixel rectangle
    rect = Rect(0, 0, 120, 120)

    -- Push it to the center of the screen
    rect.x = (screen.bounds.w - rect.w) / 2
    rect.y = (screen.bounds.h - rect.h) / 2

    -- Draw our rectangle! (square?)
    screen.pen = Pen(0, 255, 0)
    screen.rectangle(rect)

    -- Create a point in the middle of the screen for our circle center
    center = Point(screen.bounds.w / 2, screen.bounds.h / 2)

    -- Draw our circle!
    screen.pen = Pen(255, 0, 0)
    screen.circle(center, 50)

    -- Oh heck, no, not a polygon!
    points = {center + Point(-50, 80), center + Point(0, 30), center + Point(50, 80)}

    screen.pen = Pen(0, 0, 255)
    screen.polygon(points)

    -- What about some regular polygons?
    screen.pen = Pen(255, 0, 255)
    screen.polygon(regular_polygon(center, 5, 25, time / 25))

    -- Drawn into a surface!
    surface.pen = Pen(255, 255, 0)
    surface:polygon(regular_polygon(Point(surface.bounds.w / 2, surface.bounds.h / 2), 7, 25, time / 10))

    -- And blitted to the screen
    screen.blit(surface, surface_rect, Point((screen.bounds.w - surface.bounds.w) / 2, 5))
end
