#include "Drawing.h"
#include "Point.h"

HMENU Drawing::POPUP;

void Drawing::DrawPoint(float x, float y) {
    glPointSize(5.f);
    glBegin(GL_POINTS);
    glColor3f(0.f, 1.f, 0.f);
    glVertex2f(x, y);
    glEnd();
}

void Drawing::DrawLine(float p1x, float p1y, float p2x, float p2y) {
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(p1x, p1y);
    glVertex2f(p2x, p2y);
    glEnd();
}

float _distance(float p1x, float p1y, float p2x, float p2y) {
    float distX = p2x - p1x;
    float distY = p2y - p1y;
    return std::sqrt(distX * distX + distY * distY);
}

Point* Drawing::findTengent(Point* pStart, Point* pEnd, Point* prevStart, Point* prevEnd) {
    if (prevStart->GetType() == LINE) return prevStart;
    if (pStart->midPoint == NULL) return prevStart;

    Point* center = pStart->midPoint;
    float Sx = center->GetX();
    float Sy = center->GetY();

    float x1 = prevStart->GetX(), y1 = prevStart->GetY();
    float x2 = pStart->GetX(), y2 = pStart->GetY();
    float x3 = pEnd->GetX(), y3 = pEnd->GetY();

    float Dx = x2 - x1;
    float Dy = y2 - y1;

    float Nx = x2 - Sx;
    float Ny = y2 - Sy;

    float lengthN = sqrt(Nx * Nx + Ny * Ny);
    Nx /= lengthN;
    Ny /= lengthN;

    float Tx = -Ny;
    float Ty = Nx;

    float orientation = Dx * Ty - Dy * Tx;
    if (orientation > 0) {
        Tx = -Tx;
        Ty = -Ty;
    }

    float scale = 10.0f;
    float x4 = x2 + Tx * scale;
    float y4 = y2 + Ty * scale;

    return new Point(LINE, x4, y4);
}

void Drawing::findCircleFromPoints(Point* pStart, Point* pEnd, Point* prevStart, Point* prevEnd, float& centerX, float& centerY, float& r, float& m1, float& m2, float& m3, float& m4) {
    Point* tengent = findTengent(pStart, pEnd, prevStart, prevEnd);
    float x1 = tengent->GetX(), y1 = tengent->GetY();
    float x2 = pStart->GetX(), y2 = pStart->GetY();
    float x3 = pEnd->GetX(), y3 = pEnd->GetY();

    m1 = (y2 - y1) / (x2 - x1);
    m3 = -(1 / m1);
    m2 = (y3 - y2) / (x3 - x2);
    m4 = -(1 / m2);

    float b3 = y2 - m3 * x2;
    float x4 = (-m4 * x3 + y3 + m3 * x2 - y2) / (m3 - m4);
    float y4 = m3 * x4 + b3;

    r = _distance(x4, y4, x2, y2) / 2;
    centerX = (x2 + x4) / 2;
    centerY = (y2 + y4) / 2;
}

void Drawing::DrawArc(Point* pStart, Point* pEnd, Point* prevStart, Point* prevEnd) {
    Point* tengent = findTengent(pStart, pEnd, prevStart, prevEnd);
    float x1 = tengent->GetX(), y1 = tengent->GetY();
    float x2 = pStart->GetX(), y2 = pStart->GetY();
    float x3 = pEnd->GetX(), y3 = pEnd->GetY();

    float m1, m2, m3, m4;
    float centerX, centerY;
    float r;

    Drawing::findCircleFromPoints(pStart, pEnd, prevStart, prevEnd, centerX, centerY, r, m1, m2, m3, m4);

    float startAngle = atan2(pStart->GetY() - centerY, pStart->GetX() - centerX);
    float endAngle = atan2(pEnd->GetY() - centerY, pEnd->GetX() - centerX);

    if (x1 < x2) {
        if (m1 > 0 && startAngle < 0 && startAngle > endAngle) endAngle += 2 * PI;
        else if (m1 > 0 && startAngle > 0 && endAngle > startAngle) endAngle -= 2 * PI;
        else if (m1 <= 0 && startAngle < 0 && startAngle > endAngle) endAngle += 2 * PI;
        else if (m1 <= 0 && startAngle > 0 && endAngle > startAngle) endAngle -= 2 * PI;
    }
    else {
        if (m1 > 0 && startAngle > 0 && startAngle > endAngle) endAngle += 2 * PI;
        else if (m1 > 0 && startAngle < 0 && endAngle > startAngle) endAngle -= 2 * PI;
        else if (m1 <= 0 && startAngle > 0 && startAngle > endAngle) endAngle += 2 * PI;
        else if (m1 <= 0 && startAngle < 0 && endAngle > startAngle) endAngle -= 2 * PI;
    }

    pEnd->midPoint = new Point(LINE, centerX, centerY);

    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= ARC_NUM_SEGMENTS; i++) {
        float angle = startAngle + i * (endAngle - startAngle) / ARC_NUM_SEGMENTS;
        float x = centerX + r * cos(angle);
        float y = centerY + r * sin(angle);

        if (i == ARC_NUM_SEGMENTS / 2) {
            glEnd();
            Drawing::DrawPoint(x, y);
            glColor3f(1, 0, 0);
            glBegin(GL_LINE_STRIP);
        }

        glVertex2f(x, y);
    }
    glEnd();
}

void Drawing::ShowContextMenu(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (!POPUP) {
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

float Drawing::TranslatePointWinToGL(int point, int pointMax) {
    float ret = static_cast<float>(point) - static_cast<float>(pointMax) / 2.f;
    ret /= pointMax / 2.f;
    return ret;
}
