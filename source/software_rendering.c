
global_variable const f32 scale = 0.01f; // Hardcoded


internal void ClearScreen (u32 color)
{
    u32* pixel = render_buffer.pixels;
    
    for (int y = 0; y < render_buffer.height; y++)
    {
        for (int x = 0; x < render_buffer.width; x++)
        {
            *pixel++ = color;
        }
    }
}

internal void DrawRectInPixels (int x0, int y0, int x1, int y1, u32 color)
{
    
    x0 = Clamp(x0, 0,  render_buffer.width);
    x1 = Clamp(x1, x0, render_buffer.width);
    
    
    y0 = Clamp(y0, 0,  render_buffer.height);
    y1 = Clamp(y1, y0, render_buffer.height);
    
    
    for (int y = y0; y < y1; y++)
    {
        u32* pixel = render_buffer.pixels + x0 + render_buffer.width * y;
        for (int x = x0; x < x1; x++)
        {
            *pixel++ = color;
        }
    }
}

internal f32 CalculateAspectMultiplier() 
{
    f32 aspect_multiplier = (f32) render_buffer.height;
    if ((f32) render_buffer.width / (f32) render_buffer.height < 1.77f)
        aspect_multiplier = (f32) render_buffer.width / 1.77f;
    
    return aspect_multiplier;
}

internal void DrawRect (v2 p, v2 half_size, u32 color)
{
    f32 aspect_multiplier = CalculateAspectMultiplier();
    
    half_size.x *= aspect_multiplier * scale;
    half_size.y *= aspect_multiplier * scale;
    
    p.x *= aspect_multiplier * scale;
    p.y *= aspect_multiplier * scale;
    
    p.x += (f32) render_buffer.width  * 0.5f;
    p.y += (f32) render_buffer.height * 0.5f;
    
    
    int x0 = (int) (p.x - half_size.x);
    int y0 = (int) (p.y - half_size.y);
    int x1 = (int) (p.x + half_size.x);
    int y1 = (int) (p.y + half_size.y);
    
    DrawRectInPixels(x0, y0, x1, y1, color);
    
}


internal void ClearScreenAndDrawRect(v2 p, v2 half_size, u32 draw_color, u32 clear_color)
{
    f32 aspect_multiplier = CalculateAspectMultiplier();
    
    half_size.x *= aspect_multiplier * scale;
    half_size.y *= aspect_multiplier * scale;
    
    p.x *= aspect_multiplier * scale;
    p.y *= aspect_multiplier * scale;
    
    p.x += (f32) render_buffer.width  * 0.5f;
    p.y += (f32) render_buffer.height * 0.5f;
    
    
    int x0 = (int) (p.x - half_size.x);
    int y0 = (int) (p.y - half_size.y);
    int x1 = (int) (p.x + half_size.x);
    int y1 = (int) (p.y + half_size.y);
    
    DrawRectInPixels(x0, y0, x1, y1, draw_color);
    
    DrawRectInPixels(0, 0, x0, render_buffer.height, clear_color);
    DrawRectInPixels(x1, 0, render_buffer.width, render_buffer.height, clear_color);
    
    DrawRectInPixels(x0, 0, x1, y0, draw_color);
    DrawRectInPixels(x0, y1, render_buffer.width, render_buffer.height, clear_color);
}

internal v2 PixelsToWorld (v2i pixels_coord)
{
    v2 result;
    
    f32 aspect_multiplier = CalculateAspectMultiplier();
    
    result.x = (f32)pixels_coord.x -  (f32)render_buffer.width  * 0.5f;
    result.y = (f32)pixels_coord.y -  (f32)render_buffer.height * 0.5f;;
    
    
    result.x /= aspect_multiplier * scale;
    result.y /= aspect_multiplier * scale;
    
    
    return result;
}

