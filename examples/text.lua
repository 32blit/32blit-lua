variable_width = false
alignment = TextAlign.top_left

alignment_to_string = {
    [TextAlign.bottom_left] = "align: bottom_left",
    [TextAlign.bottom_right] = "align: bottom_right",
    [TextAlign.top_left] = "align: top_left",
    [TextAlign.top_right] = "align: top_right",
    [TextAlign.center_center] = "align: center_center",
    [TextAlign.center_left] = "align: center_left",
    [TextAlign.center_right] = "align: center_right",
    [TextAlign.top_center] = "align: top_center",
    [TextAlign.bottom_center] = "align: bottom_center"
}

function init()
    set_screen_mode(ScreenMode.hires)
end

function update(time)
    alignment = TextAlign.top_left

    if buttons.state & Button.DOWN ~= 0 then
        alignment = alignment + TextAlign.bottom
    elseif buttons.state & Button.UP == 0 then
        alignment = alignment + TextAlign.center_v
    end
    if buttons.state & Button.RIGHT ~= 0 then
        alignment = alignment + TextAlign.right
    elseif buttons.state & Button.LEFT == 0 then
        alignment = alignment + TextAlign.center_h
    end

    if buttons.released & Button.A ~= 0 then
        variable_width = not variable_width
    end
end

function render(time)
    screen.clip = Rect(Point(0, 0), screen.bounds)
    screen.pen = Pen(0, 0, 0)
    screen.clear()

    screen.alpha = 255
    screen.pen = Pen(255, 255, 255)
    screen.rectangle(Rect(0, 0, 320, 14))
    screen.pen = Pen(0, 0, 0)
    screen.text("Text Rendering", minimal_font, Point(5, 4))

    text_rect = Rect(20, 20, 120, 80)
    screen.pen = Pen(64, 64, 64)
    screen.rectangle(text_rect)

    text = "This is some aligned text!\nUse the dpad to change the alignment\nand A to toggle variable-width."
    text = screen.wrap_text(text, text_rect.w, minimal_font, variable_width)

    screen.pen = Pen(255, 255, 255)
    screen.text(text, minimal_font, text_rect, variable_width, alignment)

    screen.text(alignment_to_string[alignment], minimal_font, Point(80, 102), true, TextAlign.center_h)


    size = screen.measure_text(text, minimal_font, variable_width)
    screen.text("bounds: " .. size.w .. "x" .. size.h, minimal_font, Point(80, 110), true, TextAlign.center_h)


    text_rect.x  = text_rect.x + 160
    screen.pen = Pen(64, 64, 64)
    screen.rectangle(text_rect)

    clip_rect = Rect(text_rect.x + 30 + 30 * math.cos(time / 1000.0), text_rect.y, 60, 80)

    text = "This text is clipped!\nIt's slightly hard to read since half of it is missing."
    text = screen.wrap_text(text, text_rect.w, minimal_font, variable_width)


    screen.pen = Pen(255, 255, 255)
    screen.clip = clip_rect
    screen.text(text, minimal_font, text_rect, true, TextAlign.center_center)
    screen.clip = Rect(Point(0, 0), screen.bounds)


    text_rect.x = text_rect.x - 160
    text_rect.y = text_rect.y + 120
    screen.pen = Pen(64, 64, 64)
    screen.rectangle(text_rect)

    text = "This text uses\nthe fancy\noutline font."
    text = screen.wrap_text(text, text_rect.w, minimal_font, variable_width)

    screen.pen = Pen(255, 255, 255)
    screen.text(text, outline_font, text_rect, variable_width, alignment)


    -- Alignment around a Point rather than a Rect
    text_point = Point(240, 180);
  
    screen.pen = Pen(64, 64, 64);
    screen.line(Point(text_point.x - 20, text_point.y - 0), Point(text_point.x + 20, text_point.y + 0))
    screen.line(Point(text_point.x - 0, text_point.y - 20), Point(text_point.x + 0, text_point.y + 20))
  
    text = "This text is\naligned to a\nPoint instead\nof a Rect."
    text = screen.wrap_text(text, text_rect.w, minimal_font, variable_width)

    screen.pen = Pen(255, 255, 255)
    screen.text(text, minimal_font, text_point, variable_width, alignment)
end
