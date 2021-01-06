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
    
function render(time)
    blit.pen(Pen(255, 0, 255))
    blit.pixel(Point(10, 10))
    blit.pixel(Point(11, 10))
    blit.pixel(Point(10, 11))
    blit.pixel(Point(11, 11))
    local x = math.random(0, 319)
    local y = math.random(0, 239)
    blit.pixel(Point(x, y))
    local i = math.random(0, 16)
    blit.sprite(i, Point(x, y))
    --blit.debug(time)
end
