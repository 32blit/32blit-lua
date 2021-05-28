text_rect = Rect(20, 20, 120, 80)
clip_rect = Rect(20, 20, 120, 80)

function init()
end

function update(time)
end

function render(time)
    screen.pen = Pen(0, 0, 0)
    screen.clear()

    clip_rect.w = 60 + (60.0 * math.cos(time / 1000.0))

    screen.clip = clip_rect

    screen.pen = Pen(255, 255, 255)
    text = "This text is clipped!\nIt's slightly hard to read since half of it is missing."
    text = screen.wrap_text(text, text_rect.w, outline_font)
    screen.text(text, minimal_font, text_rect, TextAlign.top_left)
end
