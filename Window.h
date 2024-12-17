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
	std::string title;
	ATOM WINDOW_CLASS;
	HWND windowPtr;
	void _initWndClass();
	void _initWindow();
	void _initGL();
	void _eventHandler();
public:
	using EventHandler = std::function<void(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)>;
	Window(int width, int height, std::string title);
	~Window() = default;
	void Update();
	inline int GetWidth() { return this->width; }
	inline int GetHeight() { return this->height; }

	HWND GetWindow(); 
	EventHandler lbClickHandle;
	EventHandler rbClickHandle;
	EventHandler contextMenuHandle;
	EventHandler mouseMoveHandle;
	EventHandler keyboardHandle;
	EventHandler focusHandle;
	EventHandler unfocusHandle;

	Program* programInstance = nullptr;

};

