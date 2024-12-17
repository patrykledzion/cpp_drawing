#include "Program.h"

Program::Program() 
{
	this->window = new Window(800, 600, "Okno");
	this->window->programInstance = this;
	this->points = new std::vector<Point*>();
	this->history = new std::vector<Point*>();
	this->mousePos = new Point(LINE, 0, 0);
	this->window->lbClickHandle = ((Window::EventHandler)[this](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {this->lbClickHandle(hWnd, uMsg, wParam, lParam); });
	this->window->rbClickHandle = ((Window::EventHandler)[this](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {this->rbClickHandle(hWnd, uMsg, wParam, lParam); });
	this->window->contextMenuHandle = ((Window::EventHandler)[this](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {this->contextMenuHandle(hWnd, uMsg, wParam, lParam); });
	this->window->mouseMoveHandle = ((Window::EventHandler)[this](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {this->mouseMoveHandle(hWnd, uMsg, wParam, lParam); });
	this->window->keyboardHandle = ((Window::EventHandler)[this](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {this->keyboardHandle(hWnd, uMsg, wParam, lParam); });
	this->window->focusHandle = ((Window::EventHandler)[this](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {this->focusHandle(hWnd, uMsg, wParam, lParam); });
	this->window->unfocusHandle = ((Window::EventHandler)[this](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {this->unfocusHandle(hWnd, uMsg, wParam, lParam); });
}
void Program::Update()
{
    this->window->Update();

    if (!this->running) return;

    if (this->points->size() < 2)
        this->SetActiveType(LINE);
}

void Program::Draw()
{
    if (!this->running) return;

    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < this->points->size(); i++)
    {
        auto p1 = this->points->at(i);
        auto pPrev = i - 1 >= 0 ? this->points->at(i - 1) : nullptr;
        auto p2 = i + 1 < this->points->size() ? this->points->at(i + 1) : nullptr;

        p1->Draw();

        if (p2 != nullptr && p2->GetType() == LINE)
            Drawing::DrawLine(p1->GetX(), p1->GetY(), p2->GetX(), p2->GetY());
        else if (p2 != nullptr && pPrev != nullptr && p2->GetType() == ARC)
            Drawing::DrawArc(p1, p2, pPrev, p1);
    }

    if (this->points->size() > 0 && lineStarted)
    {
        Point* lastPoint = this->points->at(this->points->size() - 1);
        if (this->activeType == LINE)
            Drawing::DrawLine(lastPoint->GetX(), lastPoint->GetY(), mousePos->GetX(), mousePos->GetY());
        else if (this->points->size() > 1 && this->activeType == ARC)
        {
            Point* prevPoint = this->points->at(this->points->size() - 2);
            Drawing::DrawArc(lastPoint, mousePos, prevPoint, lastPoint);
        }
    }

    glFlush();
}

void Program::PushHistory()
{
    if (this->points->size() < 1) return;
    Point* p = this->points->at(this->points->size() - 1);
    this->history->push_back(p);
    this->points->pop_back();
}

void Program::PopHistory()
{
    if (this->history->size() < 1) return;
    Point* p = this->history->at(this->history->size() - 1);
    this->points->push_back(p);
    this->history->pop_back();
}

void Program::SetActiveType(LINE_TYPE type)
{
    auto pop = Drawing::POPUP;
    MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
    mii.fMask = MIIM_STATE;

    GetMenuItemInfo(pop, OPTION_ARC, FALSE, &mii);
    CheckMenuItem(pop, OPTION_LINE, type == LINE ? MF_CHECKED : MF_UNCHECKED);
    CheckMenuItem(pop, OPTION_ARC, type == ARC ? MF_CHECKED : MF_UNCHECKED);

    this->activeType = type;
}

void Program::StartLine()
{
    this->lineStarted = true;
}

void Program::FinishLine()
{
    this->lineStarted = false;
}

void Program::lbClickHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    float glPointX = Drawing::TranslatePointWinToGL(GET_X_LPARAM(lParam), this->window->GetWidth());
    float glPointY = Drawing::TranslatePointWinToGL(GET_Y_LPARAM(lParam), this->window->GetHeight()) * -1;

    if (!this->lineStarted)
    {
        this->points->clear();
        this->history->clear();
        this->lineStarted = true;
    }

    this->points->push_back(new Point(activeType, glPointX, glPointY));
    this->history->clear();
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
        this->SetActiveType(LINE);
        break;
    case OPTION_ARC:
        this->SetActiveType(ARC);
        break;
    case OPTION_CONTINUE:
        this->StartLine();
        break;
    case OPTION_FINISH:
        this->FinishLine();
        break;
    case OPTION_UNDO:
        this->PushHistory();
        break;
    case OPTION_REDO:
        this->PopHistory();
        break;
    }
}

void Program::mouseMoveHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    this->mousePos->SetX(Drawing::TranslatePointWinToGL(GET_X_LPARAM(lParam), this->window->GetWidth()));
    this->mousePos->SetY(Drawing::TranslatePointWinToGL(GET_Y_LPARAM(lParam), this->window->GetHeight()) * -1);
}

void Program::keyboardHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int keyCode = wParam;
    switch (keyCode)
    {
    case VK_SPACE:
        this->SetActiveType(this->activeType == LINE ? ARC : LINE);
        break;
    case VK_RETURN:
        this->FinishLine();
        break;
    case VK_ESCAPE:
        this->StartLine();
        break;
    case 90:
        if ((GetKeyState(VK_CONTROL) & 0x8000) && !(GetKeyState(VK_SHIFT) & 0x8000))
            this->PushHistory();
        else if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000))
            this->PopHistory();
        break;
    }
}

void Program::unfocusHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    this->running = false;
}

void Program::focusHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    this->running = true;
}