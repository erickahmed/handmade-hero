/* =================================================================================
    This file is a part of the Handmade Hero project series made by Casey Muratori
    that I am building using his videos as a reference and adding a personal touch!

    - INFORMATIONS -
    File: handmade.cpp
    Author: Erick Ahmed
    Creation: 08/03/2022
    Last revision: 21/03/2022
=================================================================================== */

#include "framework.h"

//#define _CRTDBG_MAP_ALLOC


WNDPROC Wndproc;

LRESULT CALLBACK WndCallback(
    HWND Window,
    UINT Message,
    WPARAM WParam,
    LPARAM LParam
) {
    LRESULT Result;

    switch (Message)
    {
    case WM_SIZE:
        OutputDebugStringA("WM_SIZE\n");
        Result = 0;
        break;
    
    case WM_DESTROY:
        OutputDebugStringA("WM_DESTROY\n");
        Result = 0;
        break;

    case WM_CLOSE:
        OutputDebugStringA("WM_CLOSE\n");
        Result = 0;
        break;
    
    case WM_ACTIVATEAPP:
        OutputDebugStringA("WM_ACTIVATEAPP\n");
        Result = 0;
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT Paint;

        HDC DeviceContext = BeginPaint(Window, &Paint);

        int X = Paint.rcPaint.left;
        int Y = Paint.rcPaint.top;
        int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
        int Width = Paint.rcPaint.right - Paint.rcPaint.left;

        PatBlt(DeviceContext, X, Y, Height, Width, WHITENESS);
        EndPaint(Window, &Paint);

        Result = 0;
    }   break;

    default:
        OutputDebugStringA("default\n");
        Result = DefWindowProc(Window, Message, WParam, LParam);
        break;
    }

    return(Result);
}

int WINAPI WinMain(
    HINSTANCE   Instance,
    HINSTANCE   PrevInstance,
    LPSTR       CmdLine,
    int         ShowCode
) {
    WNDCLASSEXA WindowClass = {};
    
    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = WndCallback;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = NULL;                               // TODO: add icon
    WindowClass.lpszMenuName = "Handmade Hero";             // FIXME: in debug output is random char
    WindowClass.lpszClassName = "HandmadeHeroWindowClass";
    
    if (RegisterClassExA(&WindowClass)) {
        HWND WindowHandle = CreateWindowExA(
            0,
            WindowClass.lpszClassName,
            WindowClass.lpszMenuName,           // SEE: line 71
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            Instance,
            0
        );

        if (WindowHandle != NULL) {
            MSG Message;

            for (;;) {
                BOOL MessageResult = GetMessage(&Message, NULL, 0, 0);
                if (MessageResult > 0) {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                } else break;
            }
        }
        else {
            // TODO: logging and error manager
        }
    }
    else {
        // TODO: logging and error manager
    }

    return 0;
}