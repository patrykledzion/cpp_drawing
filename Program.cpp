#include "Program.h"

Program::Program() 
{
	this->window = new Window(800, 600, L"Okno");
	this->window->lbClickHandle = ((Window::EventHandler)[this](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {this->lbClickHandle(hWnd, uMsg, wParam, lParam); });
	this->window->rbClickHandle = ((Window::EventHandler)[this](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {this->rbClickHandle(hWnd, uMsg, wParam, lParam); });
	this->window->contextMenuHandle = ((Window::EventHandler)[this](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {this->contextMenuHandle(hWnd, uMsg, wParam, lParam); });
}

void Program::Update()
{
	this->window->Update();
}

void Program::Draw()
{
	Drawing::DrawLine(-1, -1, 1, 1);
}

void Program::lbClickHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MessageBox(this->window->GetWindow(), L"CLICK", L"LCLICK", MB_OK);
}

void Program::rbClickHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Drawing::ShowContextMenu(hWnd, uMsg, wParam, lParam);
}

void Program::contextMenuHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto pop = Drawing::POPUP;
	switch (LOWORD(wParam))
	{
	case OPTION_LINE:
	{
		MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
		mii.fMask = MIIM_STATE;
		GetMenuItemInfo(pop, OPTION_ARC, FALSE, &mii);
		CheckMenuItem(pop, OPTION_LINE, MF_CHECKED);
		CheckMenuItem(pop, OPTION_ARC, MF_UNCHECKED);
		break;
	}
	case OPTION_ARC:
	{
		MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
		mii.fMask = MIIM_STATE;
		GetMenuItemInfo(pop, OPTION_ARC, FALSE, &mii);
		CheckMenuItem(pop, OPTION_LINE, MF_UNCHECKED);
		CheckMenuItem(pop, OPTION_ARC, MF_CHECKED);
		break;
	}
	case OPTION_CONTINUE:
		break;
	case OPTION_FINISH:
		break;
	case OPTION_UNDO:
		break;
	case OPTION_REDO:
		break;
	}
}