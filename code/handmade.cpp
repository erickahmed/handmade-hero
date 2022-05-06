/* =================================================================================
    This file is a part of the Handmade Hero project series made by Casey Muratori
    that I am building using his videos as a reference and adding a personal touch!

    - INFORMATIONS -
    File: handmade.cpp
    Author: Erick Ahmed
    Creation: 08/03/2022
    Last revision: 04/05/2022
    Reference: Handmade Hero Day 004
=================================================================================== */

#include "framework.h"

static bool Running;
static BITMAPINFO BitmapInfo;
static void* BitmapMemory;
static int BitmapWidth, BitmapHeight;
static int BytesPerPixel = 4;

static void RenderToWindow(int XOffset, int YOffset)
{
    int Width  = BitmapWidth;
    int Height = BitmapHeight;

    int Pitch = Width * BytesPerPixel;
    uint8_t *Row = (uint8_t *)BitmapMemory;
    
    for (int Y = 0; Y < BitmapHeight; ++Y)
    {
        uint32_t *Pixel = (uint32_t *)Row;
        for (int X = 0; X < BitmapWidth; ++X)
        {
            /* 0xBBGGRRXX */

            uint8_t Red = 128;
            uint8_t Blue = X + XOffset;
            uint8_t Green = Y + YOffset;

            *Pixel++ = Red << 16 | Green << 8 | Blue;
        }
        Row += Pitch;
    }
}

static void Win32ResizeDIBSection(int Width, int Height)
{
    if (BitmapMemory) VirtualFree(BitmapMemory, 0, MEM_RELEASE);

    BitmapWidth  = Width;
    BitmapHeight = Height;

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = BitmapWidth;
    BitmapInfo.bmiHeader.biHeight = -BitmapHeight;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    int BitmapMemorySize = Width * Height * BytesPerPixel;
    BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

static void WindowUpdate(HDC DeviceContext, RECT *ClientRect, int Width, int Height)
{
    int WindowWidth  = ClientRect->right  - ClientRect->left;
    int WindowHeight = ClientRect->bottom - ClientRect->top;

    StretchDIBits(
        DeviceContext,
        0, 0, BitmapWidth, BitmapHeight,
        0, 0, WindowWidth, WindowHeight,
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

        //int X = Paint.rcPaint.left;
        //int Y = Paint.rcPaint.top;
        int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
        int Width  = Paint.rcPaint.right  - Paint.rcPaint.left;

        RECT ClientRect;
        GetClientRect(Window, &ClientRect);

        WindowUpdate(DeviceContext, &ClientRect, Width, Height);

        EndPaint(Window, &Paint);

        Result = 0;
        break;
    }

    default:
        Result = DefWindowProcA(Window, Message, WParam, LParam);
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
    WindowClass.lpszMenuName = "Handmade Hero";
    WindowClass.lpszClassName = "HandmadeHeroWindowClass";

    if (RegisterClassExA(&WindowClass))
    {
        HWND WindowHandle = CreateWindowExA(
            0,
            WindowClass.lpszClassName,
            WindowClass.lpszMenuName,
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
            int XOffset = 0;
            int YOffset = 0;

            while(Running)
            {
                MSG Message;

                while (PeekMessageA(&Message, NULL, 0, 0, PM_REMOVE))
                {
                    if (Message.message == WM_QUIT) Running = false;

                    TranslateMessage(&Message);
                    DispatchMessage(&Message);

                    RenderToWindow(XOffset, YOffset);

                    HDC DeviceContext = GetDC(WindowHandle);
                    RECT ClientRect;
                    GetClientRect(WindowHandle, &ClientRect);
                    int WindowWidth = ClientRect.right - ClientRect.left;
                    int WindowHeight = ClientRect.bottom - ClientRect.top;
                    WindowUpdate(DeviceContext, &ClientRect, WindowWidth, WindowHeight);
                    ReleaseDC(WindowHandle, DeviceContext);
                    
                    XOffset++;
                    YOffset++;
                }
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