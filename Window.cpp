#include "Window.h"
#include "Program.h"
#define OPTION_LINE 101
#define OPTION_ARC 102
#define OPTION_FINISH 103
#define OPTION_CONTINUE 104
#define OPTION_UNDO 105
#define OPTION_REDO 106

LRESULT Wndproc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	Window* wnd = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
	switch (uMsg)
	{
	case WM_RBUTTONUP:
	{
		wnd->rbClickHandle(hWnd, uMsg, wParam, lParam); 
		break;
	}

	case WM_LBUTTONUP:
	{ 
		wnd->lbClickHandle(hWnd, uMsg, wParam, lParam);
		break;
	}

	case WM_MOUSEMOVE: 
	{
		wnd->mouseMoveHandle(hWnd, uMsg, wParam, lParam);
		break;
	}
	
	case WM_KEYDOWN: 
	{
		wnd->keyboardHandle(hWnd, uMsg, wParam, lParam);
		break;
	}

	case WM_COMMAND:
	{
		wnd->contextMenuHandle(hWnd, uMsg, wParam, lParam);
		break;
	}

	case WM_KILLFOCUS:
	{
		wnd->unfocusHandle(hWnd, uMsg, wParam, lParam);
		break;
	}

	case WM_SETFOCUS:
	{
		if (!wnd->programInstance)break;
		wnd->focusHandle(hWnd, uMsg, wParam, lParam);
		break;
	}

	default: 
		return DefWindowProcA(hWnd, uMsg, wParam, lParam);
	}
	
}

void Window::_initWndClass()
{
	if (this->WINDOW_CLASS != NULL)return;
	WNDCLASS wndClass;
	wndClass.style = 0;
	wndClass.lpfnWndProc = &Wndproc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = NULL;
	wndClass.hIcon = NULL;
	wndClass.hCursor= NULL;
	wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"Okno";

	this->WINDOW_CLASS = (ATOM)RegisterClass(&wndClass);
}

void Window::_initWindow()
{
	
 

	if (Window::WINDOW_CLASS == NULL)return;
	HWND hwnd = CreateWindowEx(
		0,
		(LPCWSTR)Window::WINDOW_CLASS,
		L"",
		WS_TILEDWINDOW,
		100, 100, //POS
		this->width, this->height,
		NULL, NULL, NULL, NULL
	);

	if (hwnd == NULL)return; 
	this->windowPtr = hwnd;
	SetWindowLongPtrW(this->windowPtr, GWLP_USERDATA, (LONG_PTR)this);
	ShowWindow(hwnd, SW_NORMAL);
}

void Window::_initGL()
{
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	HDC hdc = GetDC(this->windowPtr);
	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);

	HGLRC hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);

	RECT rcCli;
	GetClientRect(WindowFromDC(hdc), &rcCli);
	this->width = rcCli.right;
	this->height = rcCli.bottom;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 

}

void Window::_eventHandler()
{
	MSG msg;
	if (PeekMessageA(&msg, 0, 0, 0, 1))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

Window::Window(int width, int height, std::string title) : width(width), height(height), title(title)
{
	this->WINDOW_CLASS = (ATOM)NULL;
	this->_initWndClass();
	this->_initWindow();
	this->_initGL();
}

void Window::Update()
{
	this->_eventHandler();
	SwapBuffers(GetDC(this->windowPtr));
}
 
HWND Window::GetWindow()
{
	return this->windowPtr;
}
 