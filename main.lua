function init()
    blit.debug("hello world")
    blit.load_sprites("dingbads.bin")

    local point = Point(10, 10);
    local point1 = Point(point.x, point.y)
    local test = Pen(255, 0, 255)

    blit.pen(255, 255, 255)
end
    
function update(time)
end

function random_sprites(time)
    local x = math.random(0, 319)
    local y = math.random(0, 239)
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
