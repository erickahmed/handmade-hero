/* =================================================================================
    This file is a part of the Handmade Hero project series made by Casey Muratori
    that I am building using his videos as a reference and adding a personal touch!

    - INFORMATIONS -
    File: handmade.cpp
    Author: Erick Ahmed
    Creation: 08/03/2022
    Last revision: 25/03/2022
=================================================================================== */

#include "framework.h"

//#define _CRTDBG_MAP_ALLOC

static bool Running;
static BITMAPINFO BitmapInfo;
static void* BitmapMemory;
static HBITMAP BitmapHandle;
static HDC BitmapDeviceContext;

static void Win32ResizeDIBSection(int Width, int Height)
{
    if (BitmapHandle) DeleteObject(BitmapHandle);
    if (!BitmapDeviceContext) BitmapDeviceContext = CreateCompatibleDC(0);

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    BitmapInfo.bmiHeader.biHeight = Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    BitmapHandle = CreateDIBSection(
        BitmapDeviceContext,
        &BitmapInfo,
        DIB_RGB_COLORS,
        &BitmapMemory,
        NULL, NULL
    );
}

static void WindowUpdate(HDC DeviceContext, int X, int Y, int Width, int Height)
{
    StretchDIBits(
        DeviceContext,
        X, Y, Width, Height,
        X, Y, Width, Height,
        BitmapMemory,
        &BitmapInfo,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}

WNDPROC Wndproc;

LRESULT CALLBACK WndCallback(
    HWND Window,
    UINT Message,
    WPARAM WParam,
    LPARAM LParam 
) {
    LRESULT Result = 0;

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    int ClientHeight = ClientRect.bottom - ClientRect.top;
    int ClientWidth = ClientRect.right - ClientRect.left;

    switch (Message) 
    {
    case WM_SIZE:
        Win32ResizeDIBSection(ClientWidth, ClientHeight);
        break;
    
    case WM_DESTROY:
        Running = false;
        DestroyWindow(Window);
        break;

    case WM_CLOSE:
        Running = false;
        PostQuitMessage(0);
        break;
    
    case WM_ACTIVATEAPP:
        //
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT Paint;

        HDC DeviceContext = BeginPaint(Window, &Paint);

        int X = Paint.rcPaint.left;
        int Y = Paint.rcPaint.top;
        int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
        int Width  = Paint.rcPaint.right  - Paint.rcPaint.left;

        WindowUpdate(DeviceContext, X, Y, Width, Height);

        EndPaint(Window, &Paint);

        Result = 0;
        break;
    }

    default:
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
    WindowClass.lpszMenuName = "Handmade Hero";             // FIXME: it outputs some random chinese charachters... wtf is going on?!
    WindowClass.lpszClassName = "HandmadeHeroWindowClass";
    
    if (RegisterClassExA(&WindowClass))
    {
        HWND WindowHandle = CreateWindowExA(
            0,
            WindowClass.lpszClassName,
            WindowClass.lpszMenuName,           // SEE: line 85
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

        if (WindowHandle != NULL)
        {
            Running = true;
            MSG Message;

            while(Running)
            {
                BOOL MessageResult = GetMessage(&Message, NULL, 0, 0);
                if (MessageResult > 0) {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                } else break;
            }
        }
        else
        {
            // TODO: logging and error manager
        }
    }
    else
    {
        // TODO: logging and error manager
    }

    return 0;
}