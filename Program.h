#pragma once
#include "Window.h"
#include "Drawing.h"

class Program
{
private:
	Window* window;
public:
	Program();
	void Update();
	void Draw();
	void lbClickHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void rbClickHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void contextMenuHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

