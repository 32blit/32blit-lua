text_rect = Rect(20, 20, 120, 80)
clip_rect = Rect(20, 20, 120, 80)

function init()
end

function update(time)
end

function render(time)
    blit.pen(Pen(0, 0, 0))
    blit.clear()

    clip_rect.w = 60 + (60.0 * math.cos(time / 1000.0))

    blit.clip(clip_rect)

    blit.pen(Pen(255, 255, 255))
    text = "This text is clipped!\nIt's slightly hard to read since half of it is missing."
    text = blit.wrap_text(text, text_rect.w, outline_font)
    blit.text(text, minimal_font, text_rect)
end
