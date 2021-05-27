v = Vec2(160, 120)

function init()
    blit.debug("hello world")
    blit.load_sprites("dingbads.bin")

    local point = Point(10, 10);
    point.x = 1
    point.y = 2
    point = Point(point.x, point.y)

    local vec2 = Vec2(10, 10);
    vec2.x = 1
    vec2.y = 2
    vec2 = Vec2(vec2.x, vec2.y)

    local rect = Rect(0, 0, 10, 10)
    rect:contains(Point(5, 5))
    w = rect.size.w
    h = rect.size.h

    result = Rect(5, 4, 6, 6):intersection(Rect(4, 4, 6, 3))
    print(result.x, result.y, result.w, result.h)

    result = Rect(5, 5, 10, 10)
    result:inflate(1)
    print(result.x, result.y, result.w, result.h)

    clamp_point = Point(3, 3)
    clamped_point = Rect(5, 5, 10, 10):clamp(clamp_point)
    print(clamped_point.x, clamped_point.y)

    local vecA = Vec2(10, 10)
    local vecB = Vec2(10, 10)
    dot = vecA:dot(vecB)
    cross = vecA:cross(vecB)
    angle = vecA:angle(vecB)

    local size = Size(10, 10);
    size.w = 1
    size.h = 2
    size = Size(size.w, size.h)

    local pen = Pen(128, 255, 64);
    pen.r = 1
    pen.g = 2
    pen.b = 3
    pen.a = 4
    pen = Pen(pen.r, pen.g, pen.b)
    pen = Pen(pen.r, pen.g, pen.b, pen.a)

    -- force "unknown property" error
    -- TODO make this more idiomatic of Lua
    point.z = 1
end
    
function update(time)
    v.x = 160 + 160 * joystick.x
    v.y = 120 + 120 * joystick.y
end

function random_sprites(time)
    local p = Point(v)
    local x = p.x + math.random(-50, 50)
    local y = p.y + math.random(-50, 50)
    local i = math.random(0, 63)
    blit.sprite(i, Point(x, y))
end

function random_pixels(time)
    local r = math.random(0, 255)
    local g = math.random(0, 255)
    local b = math.random(0, 255)
    blit.pen(Pen(r, g, b))
    local x = math.random(0, 319)
    local y = math.random(0, 239)
    blit.pixel(Point(x, y))
end

function random_rects(time)
    local r = math.random(0, 255)
    local g = math.random(0, 255)
    local b = math.random(0, 255)
    blit.pen(Pen(r, g, b))
    local x = math.random(0, 319)
    local y = math.random(0, 239)
    local w = math.random(0, 319 - x)
    local h = math.random(0, 239 - y)
    blit.rectangle(Rect(Point(x, y), Size(w, h)))
end

function intersection_test(time)
    p = Point(v) -- convert Vec2 to Point
    blit.pen(Pen(0, 0, 0))
    blit.clear()

    blit.pen(Pen(255, 0, 0))
    blit.pixel(p)

    local x = math.random(0, 319)
    local y = math.random(0, 239)
    local w = math.random(0, 319 - x)
    local h = math.random(0, 239 - y)
    local r = Rect(Point(x, y), Size(w, h))

    if r:contains(p) then
        blit.pen(Pen(255, 0, 0))
    else
        blit.pen(Pen(0, 0, 255))
    end
    blit.rectangle(r)
end

function render(time)
    for n = 1, 10, 1
    do
        random_rects(time)
    end
    for n = 1, 100, 1
    do
        random_pixels(time)
    end
    for n = 1, 50, 1
    do
        random_sprites(time)
    end
end
