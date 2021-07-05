function init()
    set_screen_mode(ScreenMode.hires)
    -- Start the cursor in the middle of the screen
    cursor = Vec2(screen.bounds.w / 2, screen.bounds.h / 2)
end

function update(time)
    button_a_colour = Pen(100, 0, 100)
    button_b_colour = Pen(100, 100, 0)
    button_x_colour = Pen(0, 0, 100)
    button_y_colour = Pen(0, 100, 0)
    if input.state.A then
        button_a_colour = Pen(255, 0, 255)
    end
    if input.state.B then
        button_b_colour = Pen(255, 255, 0)
    end
    if input.state.X then
        button_x_colour = Pen(0, 0, 255)
    end
    if input.state.Y then
        button_y_colour = Pen(0, 255, 0)
    end

    dpad_u_colour = Pen(100, 100, 100)
    dpad_d_colour = Pen(100, 100, 100)
    dpad_l_colour = Pen(100, 100, 100)
    dpad_r_colour = Pen(100, 100, 100)
    if input.state.UP then
        dpad_u_colour = Pen(200, 200, 200)
    end
    if input.state.DOWN then
        dpad_d_colour = Pen(200, 200, 200)
    end
    if input.state.LEFT then
        dpad_l_colour = Pen(200, 200, 200)
    end
    if input.state.RIGHT then
        dpad_r_colour = Pen(200, 200, 200)
    end

    -- Update cursor position from joystick
    cursor = cursor + (joystick * 2.0)
    cursor.x = math.max(0, math.min(screen.bounds.w, cursor.x))
    cursor.y = math.max(0, math.min(screen.bounds.h, cursor.y))
end

function render(time)
    screen.pen = Pen(0, 0, 0)
    screen.clear()
    
    dpad_center = Point(screen.bounds.w / 4, screen.bounds.h / 2)
    buttons_center = Point((screen.bounds.w / 4) * 3, screen.bounds.h / 2)


    -- Draw the cross-shaped D-Pad
    dpad = Rect(0, 0, 120, 40)
    dpad.x = dpad_center.x - (dpad.w / 2)
    dpad.y = dpad_center.y - (dpad.h / 2)
    screen.pen = Pen(100, 100, 100)
    screen.rectangle(dpad)

    dpad_direction = Rect(0, 0, 40, 40)

    dpad_direction.x = dpad.tl.x
    dpad_direction.y = dpad.tl.y
    screen.pen = dpad_l_colour
    screen.rectangle(dpad_direction)

    dpad_direction.x = dpad.tr.x - dpad_direction.w
    dpad_direction.y = dpad.tr.y
    screen.pen = dpad_r_colour
    screen.rectangle(dpad_direction)

    dpad.w, dpad.h = dpad.h, dpad.w -- Swap the width/height, effectively a 90 degree rotation
    dpad.x = dpad_center.x - (dpad.w / 2)
    dpad.y = dpad_center.y - (dpad.h / 2)
    screen.pen = Pen(100, 100, 100)
    screen.rectangle(dpad)

    dpad_direction.x = dpad.tl.x
    dpad_direction.y = dpad.tl.y
    screen.pen = dpad_u_colour
    screen.rectangle(dpad_direction)

    dpad_direction.x = dpad.bl.x
    dpad_direction.y = dpad.bl.y - dpad_direction.h
    screen.pen = dpad_d_colour
    screen.rectangle(dpad_direction)

    -- Draw the 4 face buttons
    button_distance = 40 -- Distance each button circle should be from the middle
    button_size = 20 -- Radius of the button circles
    
    screen.pen = button_y_colour
    screen.circle(buttons_center + Point(-button_distance, 0), button_size)

    screen.pen = button_a_colour
    screen.circle(buttons_center + Point(button_distance, 0), button_size)

    screen.pen = button_x_colour
    screen.circle(buttons_center + Point(0, -button_distance), button_size)

    screen.pen = button_b_colour
    screen.circle(buttons_center + Point(0, button_distance), button_size)

    -- Draw the joystick cursor
    screen.pen = Pen(255, 0, 0)
    screen.circle(Point(cursor.x, cursor.y), 3)
end