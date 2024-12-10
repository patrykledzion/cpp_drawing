#include "Drawing.h"

HMENU Drawing::POPUP;

void Drawing::DrawLine(int p1x, int p1y, int p2x, int p2y)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Rysowanie linii
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f); // Kolor czerwony
	glVertex2f(p1x, p1y);    // Punkt pocz¹tkowy
	glVertex2f(p2x, p2y);      // Punkt koñcowy

	glEnd();

	glFlush();
}


void Drawing::ShowContextMenu(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//HMENU pop = Drawing::POPUP;
	if (!POPUP)
	{
		Drawing::POPUP = CreatePopupMenu();
		AppendMenu(Drawing::POPUP, MF_STRING | MF_CHECKED, OPTION_LINE, L"Line\tSpace");
		AppendMenu(Drawing::POPUP, MF_STRING | MF_UNCHECKED, OPTION_ARC, L"Arc\tSpace");
		AppendMenu(Drawing::POPUP, MF_SEPARATOR, 0, NULL);
		AppendMenu(Drawing::POPUP, MF_BYPOSITION | MF_STRING, OPTION_FINISH, L"Finish\tEnter");
		AppendMenu(Drawing::POPUP, MF_BYPOSITION | MF_STRING, OPTION_CONTINUE, L"Continue\tEsc");
		AppendMenu(Drawing::POPUP, MF_SEPARATOR, 0, NULL);
		AppendMenu(Drawing::POPUP, MF_BYPOSITION | MF_STRING, OPTION_UNDO, L"Undo\tCtrl+Z");
		AppendMenu(Drawing::POPUP, MF_BYPOSITION | MF_STRING, OPTION_REDO, L"Redo\tShift+Ctrl+Z");
	}
	POINT pt;
	GetCursorPos(&pt);
	SetForegroundWindow(hWnd);
	TrackPopupMenu(Drawing::POPUP, TPM_LEFTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
}
