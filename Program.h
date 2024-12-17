#pragma once
#include <iostream>
#include <vector>
#include "Window.h"
#include "Drawing.h"
#include "Point.h"

class Program
{
private:
	Window* window;
	std::vector<Point*>* points;
	std::vector<Point*>* history;
	Point* mousePos;
	LINE_TYPE activeType = LINE;
	bool lineStarted = true;
	bool running = true;
public:
	Program();
	~Program() = default;
	void Update();
	void Draw();
	void PushHistory();
	void PopHistory();
	inline bool isRunning() { return this->running; }

	void SetActiveType(LINE_TYPE type);
	void StartLine();
	void FinishLine();
	void lbClickHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void rbClickHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void contextMenuHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void mouseMoveHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void keyboardHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void unfocusHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void focusHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

