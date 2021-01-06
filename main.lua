function init()
    blit.debug("hello world")
    blit.load_sprites("dingbads.bin")
end
    
function update(time)
end
    
function render(time)
    blit.pixel(10, 10, 0, 255, 0)
    blit.pixel(11, 10, 0, 255, 0)
    blit.pixel(10, 11, 0, 255, 0)
    blit.pixel(11, 11, 0, 255, 0)
    local x = math.random(0, 319)
    local y = math.random(0, 239)
    blit.pixel(x, y, 0, 255, 0)
    local i = math.random(0, 16)
    blit.sprite(i, x, y)
    --blit.debug(time)
end
