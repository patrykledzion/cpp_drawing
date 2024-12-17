#pragma once
#include <Windows.h>
#include <cmath>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>


#define OPTION_LINE 101
#define OPTION_ARC 102
#define OPTION_FINISH 103
#define OPTION_CONTINUE 104
#define OPTION_UNDO 105
#define OPTION_REDO 106
#define PI 3.14159265358979323846

#define ARC_NUM_SEGMENTS 50

class Point;
class Drawing
{
private:
	static void findCircleFromPoints(Point* pStart, Point* pEnd, Point* prevStart, Point* prevEnd, float& centerX, float& centerY, float& r, float& m1, float& m2, float& m3, float& m4);
	static Point* findTengent(Point* pStart, Point* pEnd, Point* prevStart, Point* prevEnd);

public:
	static HMENU POPUP;
	static void DrawPoint(float  x, float  y);
	static void DrawLine(float  p1x, float  p1y, float  p2x, float p2y);
	static void DrawArc(Point* pStart, Point* pEnd, Point* prevStart, Point* prevEnd);
	static void ShowContextMenu(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static float TranslatePointWinToGL(int point, int pointMax); // point - windows click point, pointMax - width/height of window
	Drawing() = delete;
};
