#include <windows.h>
#include <stdio.h>

#include "program_options.h"
#include "language_layer.h"
#include "math.c"

#include "platform_common.h"

// Global running state
global_variable b32 running = true;

// Global render buffer
global_variable RenderBuffer render_buffer;

#include "software_rendering.c"
#include "game.c"

internal LRESULT CALLBACK WindowCallback(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT result = 0;
    switch (message)
    {
        case WM_CLOSE:
        case WM_DESTROY: running = false;
        break;
        
        case WM_SIZE:
        {
            // Get window dimensions
            RECT rect;
            GetClientRect(window, &rect);
            
            render_buffer.width = rect.right - rect.left;
            render_buffer.height = rect.bottom - rect.top;
            
            if (render_buffer.pixels)
                VirtualFree(render_buffer.pixels, 0, MEM_RELEASE);
            
            
            
            // Allocate buffer
            render_buffer.pixels = VirtualAlloc(0, sizeof(u32) * render_buffer.width * render_buffer.height, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            
            // Fill bitmap_info
            render_buffer.bitmap_info.bmiHeader.biSize = sizeof(render_buffer.bitmap_info.bmiHeader);
            render_buffer.bitmap_info.bmiHeader.biWidth = render_buffer.width;
            render_buffer.bitmap_info.bmiHeader.biHeight = render_buffer.height;
            render_buffer.bitmap_info.bmiHeader.biPlanes = 1; // MUST BE SET TO 1
            render_buffer.bitmap_info.bmiHeader.biBitCount = 32; // 32-bit colors
            render_buffer.bitmap_info.bmiHeader.biCompression = BI_RGB; // Uncompressed
            render_buffer.bitmap_info.bmiHeader.biSizeImage = 0;
            
            
        }
        break;
        
        default: result = DefWindowProcA(window, message, w_param, l_param);
        break;
    }
    
    
    // Handle the window the default way
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
            LPSTR lpCmdLine, int nShowCmd)
{
    // Make a window class
    WNDCLASSA window_class = {0};
    window_class.style = CS_HREDRAW | CS_VREDRAW; // Vertical and horizontal redraw
    window_class.lpfnWndProc = WindowCallback;
    window_class.lpszClassName = "Game_Window_Class";
    
    // Register the window class
    RegisterClassA(&window_class);
    
    // Create window
    HWND window = CreateWindowExA(0, window_class.lpszClassName, WINDOW_NAME,
                                  WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT,
                                  0, 0, 0, 0);
    
    HDC hdc = GetDC(window);
    
    Input input = {0};
    
    // Frame rate
    LARGE_INTEGER last_counter;
    QueryPerformanceCounter(&last_counter);
    
    LARGE_INTEGER frequency_counter_large;
    QueryPerformanceFrequency(&frequency_counter_large);
    f32 frequency_counter = frequency_counter_large.QuadPart;
    
    f32 last_dt = 0.01666f;
    
    while (running) 
    {
        // Input
        MSG message;
        while (PeekMessageA(&message, window, 0, 0, PM_REMOVE))
        {
            switch (message.message)
            {
                case WM_SYSKEYDOWN:
                case WM_SYSKEYUP:
                case WM_KEYDOWN:
                case WM_KEYUP:
                {
                    u32 vk_code = (u32) message.wParam;
                    b32 was_down = ((message.lParam & (1 << 30)) != 0);
                    b32 is_down  = ((message.lParam & (1 << 31)) == 0);
                    
#define ProcessButton(vk, b) \
                    if (vk_code == vk) \
                    { \
                        input.buttons[b].changed = is_down != input.buttons[b].is_down; \
                        input.buttons[b].is_down = is_down; \
                    }
                    
                    ProcessButton(VK_LEFT, BUTTON_LEFT);
                    ProcessButton(VK_RIGHT, BUTTON_RIGHT);
                    ProcessButton(VK_UP, BUTTON_UP);
                    ProcessButton(VK_DOWN, BUTTON_DOWN);
                }
                break;
                
                default:
                {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
                break;
            }
            
        }
        
        POINT mouse_pointer;
        GetCursorPos(&mouse_pointer);
        ScreenToClient(window, &mouse_pointer);
        
        input.mouse.x = mouse_pointer.x;
        input.mouse.y = render_buffer.height - mouse_pointer.y;
        
        
        // Simulation
        SimulateGame(&input, last_dt);
        
        // Render
        StretchDIBits(hdc, 0, 0, render_buffer.width, render_buffer.height, 0, 0, render_buffer.width, render_buffer.height, render_buffer.pixels, &render_buffer.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
        
        // Delta time calculation
        LARGE_INTEGER current_counter;
        QueryPerformanceCounter(&current_counter);
        
        last_dt = min(0.1f, (f32)((current_counter.QuadPart - last_counter.QuadPart) / frequency_counter));
        
        last_counter = current_counter;
    }
}