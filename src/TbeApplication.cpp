#include "stdafx.h"
#include "..\include\TbeApplication.h"

TBE::Application::Application(uint32_t p_width, uint32_t p_height)
{
    m_window = new BaseWindow("The Borning Game", p_width, p_height);
    m_renderer = new TbeD3DRenderer();
    m_renderer->SetTargetWindow(m_window);
}

void TBE::Application::OnParseCommandLine(LPSTR lpCmdLine, uint32_t* pWidth, uint32_t* pHeight, bool* pbFullScreen)
{
    *pWidth = m_window->GetWidth();
    *pHeight = m_window->GetHeight();
}

void TBE::Application::SetNativeWindow(HWND hWnd)
{
    m_window->OnCreate(hWnd);
}

void TBE::Application::StartUpModules()
{
    m_renderer->OnInit();
}

TBE::Application::~Application()
{
}

namespace TBE {
    static const char* const WINDOW_CLASS_NAME = "TheBorningGame";

    LRESULT CALLBACK WindowProc(HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam);

    static FrameworkWindows* dxSample = NULL;

    static bool bIsFullScreen = false;
    static bool bIsMinimized = false;
    LONG lBorderedStyle = 0;
    LONG lBorderlessStyle = 0;

    int RunApp(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, Application* p_app)
    {
        // create new DX sample
        dxSample = p_app->GetWindow();

        HWND hWnd;
        WNDCLASSEX windowClass;

        // init window class
        ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = hInstance;
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.lpszClassName = WINDOW_CLASS_NAME;
        RegisterClassEx(&windowClass);

        uint32_t Width;
        uint32_t Height;
        p_app->OnParseCommandLine(lpCmdLine, &Width, &Height, &bIsFullScreen);

        RECT windowRect = { 0, 0, (LONG)Width, (LONG)Height };
        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

        // This makes sure that in a multimonitor setup with different resolutions, get monitor info returns correct dimensions
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        // create the window and store a handle to it
        hWnd = CreateWindowEx(NULL,
            WINDOW_CLASS_NAME,    // name of the window class
            dxSample ? dxSample->GetName() : "None",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            NULL,    // we have no parent window, NULL
            NULL,    // we aren't using menus, NULL
            hInstance,    // application handle
            NULL);    // used with multiple windows, NULL

        if (p_app)
            p_app->SetNativeWindow(hWnd);

        p_app->StartUpModules();

        // show the window
        ShowWindow(hWnd, nCmdShow);
        lBorderedStyle = GetWindowLong(hWnd, GWL_STYLE);
        lBorderlessStyle = lBorderedStyle & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);

        if (bIsFullScreen)
        {
            dxSample->SetFullScreen(bIsFullScreen);
        }

        // main loop
        MSG msg = { 0 };
        while (msg.message != WM_QUIT)
        {
            // check to see if any messages are waiting in the queue
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg); // translate keystroke messages into the right format            
                DispatchMessage(&msg); // send the message to the WindowProc function
            }
            else
            {
                if (p_app && bIsMinimized == false)
                {
                    p_app->GetRenderer()->OnUpdate();
                }
            }
        }

        if (dxSample)
        {
            dxSample->OnDestroy();
            delete dxSample;
        }

        // return this part of the WM_QUIT message to Windows
        return static_cast<char>(msg.wParam);
    }


    // this is the main message handler for the program
    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        // sort through and find what code to run for the message given
        switch (message)
        {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        // When close button is clicked on window
        case WM_CLOSE:
        {
            DestroyWindow(hWnd);
            return 0;
        }

        case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE)
            {
                DestroyWindow(hWnd);
            }
            break;
        }

        case WM_SIZE:
        {
            if (dxSample)
            {
                RECT clientRect = {};
                GetClientRect(hWnd, &clientRect);
                dxSample->OnResize(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

                bIsMinimized = (IsIconic(hWnd) == TRUE);

                return 0;
            }
            break;
        }

        case  WM_SYSKEYDOWN:
        {
            if ((wParam == VK_RETURN) && (lParam & (1 << 29)))
            {
                bIsFullScreen = !bIsFullScreen;
                if (dxSample)
                {
                    dxSample->SetFullScreen(bIsFullScreen);
                }
            }
            break;
        }

        // When window goes outof focus, use this event to fall back on SDR.
        // If we don't gracefully fallback to SDR, the renderer will output HDR colours which will look extremely bright and washed out.
        // However if you want to use breakpoints in HDR mode to inspect/debug values, you will have to comment this function call.
        case WM_ACTIVATE:
        {
            if (dxSample)
            {
                dxSample->OnActivate(wParam != WA_INACTIVE);
            }

            break;
        }

        }

        if (dxSample)
        {
            MSG msg;
            msg.hwnd = hWnd;
            msg.message = message;
            msg.wParam = wParam;
            msg.lParam = lParam;
            dxSample->OnEvent(msg);
        }

        // Handle any messages the switch statement didn't
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
