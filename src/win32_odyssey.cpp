/*
* @Author: Giannis
* @Date:   2022-06-26 17:45:38
* @Last Modified by:   Giannis
* @Last Modified time: 2022-06-28 06:47:54
*/

#include "odyssey.h"
#include "win32_odyssey.h"
#include <cstdint>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    //Register the window class.
    WNDCLASS WindowClass = {};
    WindowClass.lpfnWndProc = WindowProc;
    WindowClass.hInstance = hInstance;
    WindowClass.lpszClassName = "Odyssey";

    RegisterClass(&WindowClass);

    //Create Window
    HWND hwnd = CreateWindowEx(
        0, //Optional window styles.
        "Odyssey", //Window class
        "Odyssey", //Window Text
        WS_OVERLAPPEDWINDOW, //Window style
        //Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,   //Parent Window
        NULL,   //Menu
        hInstance, //Instance Handle
        NULL //Additional application data
    );

    //Check if the window was created successfully.
    if(hwnd == NULL)
    {
        OutputDebugStringW(L"hwnd == NULL.\n");
        return -1;
    }

    if(!(LOAD_VULKAN() == VK_SUCCESS))
    {
        return -1;
    }

    LOAD_VK_GLOBAL_FUNCTIONS();

    VkOdyssey vk_odyssey = {};
    vk_init(&vk_odyssey, hInstance, hwnd);

    ShowWindow(hwnd, nCmdShow);

    MSG message = {};
    uint32_t global_running = 1;
    for(;;)
    {
        while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
        {
            if(message.message == WM_QUIT)
            {
                global_running = 0;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        if(global_running == 0)
        {
            break;
        }
    }

    vk_deinit(&vk_odyssey);

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch(uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        case WM_KEYUP:
        {
            switch(wParam)
            {
                case VK_SPACE:
                    // ToggleFullscreen(hwnd);
                break;
                default:
                    result = DefWindowProc(hwnd, uMsg, wParam, lParam);
                break;
            }

        } break;
        default:
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        break;

    }
    return result;
}
