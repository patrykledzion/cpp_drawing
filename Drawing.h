#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>


#define OPTION_LINE 101
#define OPTION_ARC 102
#define OPTION_FINISH 103
#define OPTION_CONTINUE 104
#define OPTION_UNDO 105
#define OPTION_REDO 106


template <typename T>
struct static_t {
	T member;
};

class Drawing
{
private:
	
public:
	static HMENU POPUP;
	static void DrawLine(int p1x, int p1y, int p2x, int p2y);
	static void ShowContextMenu(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	Drawing() = delete;
};
