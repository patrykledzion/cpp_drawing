#pragma once
#include <iostream>
#include <Windows.h>
#include <Windowsx.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <functional>
class Program;
class Window
{
private:
	int width;
	int height;
	LPCWSTR title;
	ATOM WINDOW_CLASS;
	HWND windowPtr;
	void _initWndClass();
	void _initWindow();
	void _initGL();
	void _eventHandler();
public:
	using EventHandler = std::function<void(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)>;
	Window(int width, int height, LPCWSTR title);
	void Update();

	HWND GetWindow(); 
	EventHandler lbClickHandle;
	EventHandler rbClickHandle;
	EventHandler contextMenuHandle;

};

