timer = Timer()

move_up = Vec2(0, -1)
move_down = Vec2(0, 1)
move_left = Vec2(-1, 0)
move_right = Vec2(1, 0)
move_stop = Vec2(0, 0)

snake = {Point(8, 6)}
apple = Point(9, 10)

direction = move_stop

score = 0

sprite_snake = Point(9, 10)
sprite_apple = Point(0, 0)

game_bounds = Rect(0, 0, 320 / 8, 240 / 8)

function random_point()
    return Point(math.random(0, game_bounds.w - 1), math.random(0, game_bounds.h - 1))
end

function restart_game()
    snake = {game_bounds.center}
    apple = random_point()
    score = 0
    direction = move_stop
end

function move(timer)
    if direction == move_stop then
        return
    end
    top = snake[#snake - 0]
    head = Point(top.x + direction.x, top.y + direction.y)
    for i,segment in ipairs(snake) do
        if segment.x == head.x and segment.y == head.y then
            print("Your head esplode. Do better.")
            restart_game()
            return
        end
    end
    snake[#snake + 1] = head
    if apple.x == head.x and apple.y == head.y then
        print("You ate apple. Nom. Nom.")
        apple = random_point()
        score = score + 1
    else
        if not game_bounds:contains(head) then
            restart_game()
            return
        end
        table.remove(snake, 1)
    end
end

function init()
    set_screen_mode(ScreenMode.hires)
    game_bounds = Rect(0, 0, screen.bounds.w / 8, screen.bounds.h / 8)
    screen.load_sprites("dingbads.bin")
    restart_game()
    timer:init(move, 100)
    timer:start()
end

function update(time)
    if direction.x == 0 then
        if buttons.pressed & Button.RIGHT ~= 0 then direction = move_right end
        if buttons.pressed & Button.LEFT ~= 0 then direction = move_left end
    end
    if direction.y == 0 then
        if buttons.pressed & Button.DOWN ~= 0 then direction = move_down end
        if buttons.pressed & Button.UP ~= 0 then direction = move_up end
    end
end

function render(time)
    screen.pen = Pen(0, 0, 0)
    screen.clear()
    for i,segment in ipairs(snake) do
        screen.sprite(sprite_snake, Point(segment.x * 8, segment.y * 8))
    end
    screen.sprite(sprite_apple, Point(apple.x * 8, apple.y * 8))
    screen.pen = Pen(255, 255, 255)
    screen.text("score: " .. score, outline_font, Point(0, 0))
end
