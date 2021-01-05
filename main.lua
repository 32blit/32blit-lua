function init()
    debug("hello world")
end
    
function update(time)
end
    
function render(time)
    pixel(10, 10, 0, 255, 0)
    pixel(11, 10, 0, 255, 0)
    pixel(10, 11, 0, 255, 0)
    pixel(11, 11, 0, 255, 0)
    local x = math.random(0, 319)
    local y = math.random(0, 239)
    pixel(x, y, 0, 255, 0)
    debug(time)
end
