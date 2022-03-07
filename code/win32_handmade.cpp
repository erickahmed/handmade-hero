/* =============================
    File: win32_handmade.cpp
    Author: Erick Ahmed
    Creation: 23/06/2021
    Last revision: xx/xx/2021
============================= */

#include <windows.h>
#include <stdint.h>

//#define internal_function static              Casey's mental wanking about keyword definition and stuff like that...
//#define local_persist static                  just for future reference in case I forget bout this
//#define global_variable static

//typedef int8_t int8;
//typedef int16_t int16;
//typedef int32_t int32;
//typedef int64_t int64;

//typedef uint8_t uint8;
//typedef uint16_t uint16;
//typedef uint32_t uint32;
//typedef uint64_t uint64;

static bool Running;
static BITMAPINFO BitmapInfo;
static void *BitmapMemory;
static int BitmapHeight;
static int BitmapWidth;
/*
PAINTSTRUCT Paint;
int X = Paint.rcPaint.left;
int Y = Paint.rcPaint.top;
*/

void Win32ResizeDIBSection(int Width, int Height) {
    if(BitmapMemory) VirtualFree(BitmapMemory, 0, MEM_RELEASE);

    BitmapWidth = Width;
    BitmapHeight = Height;

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = BitmapWidth;
    BitmapInfo.bmiHeader.biHeight = -BitmapHeight;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    int BytesPerPixel = 4;
    int BitmapMemorySize = (BitmapWidth*BitmapHeight)*BytesPerPixel;
    BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    uint8_t *Row = (uint8_t*)BitmapMemory;

    for(int Y = 0; Y < BitmapHeight; ++Y) {
        int Pitch = Width*BytesPerPixel;
        uint32_t*Pixel = (uint32_t *)BitmapMemory; 
        for(int X = 0; X < BitmapHeight; ++X) {
            *Pixel = 0;            
            ++Pixel;

            *Pixel = 0;            
            ++Pixel;

            *Pixel = 255;            
            ++Pixel;

            *Pixel = 0;            
            ++Pixel;
            }
        Row += Pitch;
    }
}

void Win32UpdateWindow(HDC DeviceContext, RECT *WindowRect, int X, int Y, int Width, int Height) {
    int WindowWidth = WindowRect -> right - WindowRect -> left;
    int WindowHeight = WindowRect -> bottom - WindowRect -> top;

    StretchDIBits(
        DeviceContext,
        0, 0, BitmapWidth, BitmapHeight,
        0, 0, WindowWidth, WindowHeight,
        BitmapMemory, //const VOID       *lpBits,
        &BitmapInfo, //const BITMAPINFO *lpbmi,
        DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK Win32MainWindowCallback(
    HWND    hWnd,
    UINT    Msg,
    WPARAM  wParam,
    LPARAM  lParam)
{
    LRESULT Result = 0;
    switch(Msg) {
        case WM_SIZE: {
            RECT ClientRect;
            GetClientRect(hWnd, &ClientRect);
            int Height = ClientRect.bottom - ClientRect.top;
            int Width = ClientRect.right - ClientRect.left;
            Win32ResizeDIBSection(Width, Height);
            
            OutputDebugStringA("WM_SIZE\n");
        } break;

        case WM_DESTROY: {
            DestroyWindow(hWnd);     //treat as error ---> recreate window
            //ALTERNATIVE: Running = false;
            OutputDebugStringA("WM_DESTROY\n");
        } break;

        case WM_CLOSE: {
            Running = false;    //handle with a message [ex: do you wanna quit bro?]
            OutputDebugStringA("WM_CLOSE\n");
        } break;

        case WM_ACTIVATEAPP: {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        case WM_PAINT: {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(hWnd, &Paint);

            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;

            RECT ClientRect;
            GetClientRect(hWnd, &ClientRect);

            Win32UpdateWindow(DeviceContext, &ClientRect, X, Y, Width, Height);
            EndPaint(hWnd, &Paint);
        } break;

        default: {
            OutputDebugStringA("default\n");
            Result = DefWindowProc(hWnd, Msg, wParam, lParam);
        } break;
    }
    //system("pause"); ---> for debugging
    return Result;
}

int WINAPI WinMain(
    HINSTANCE   hInstance,
    HINSTANCE   hPrevInstance,
    LPSTR       LpCmdLine,
    int         nCmdShow)
{
    WNDCLASS WindowClass = {};

    //WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = hInstance;
    // WindowsClass.hIcon = ;
    WindowClass.lpszClassName = "HandmadeHeroWindowClass";      //FIXME: c/c++(513)

    if(RegisterClass(&WindowClass)) {
        HWND WindowHandle = CreateWindowEx(        
            0,
            WindowClass.lpszClassName,
            "Handmade Hero",                                    //FIXME: c/c++(167)
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            hInstance,
            0);

        if(WindowHandle) {
            Running = true;
            while (Running) {
                MSG Msg;
                BOOL MessageResult =  GetMessage(&Msg, 0, 0, 0);
                if(MessageResult > 0) {
                    TranslateMessage(&Msg);
                    DispatchMessage(&Msg);
                } else break;
            }
        } else {
            //TODO: logging
        }
    } else {
        //TODO: logging
    }
    return 0;
}