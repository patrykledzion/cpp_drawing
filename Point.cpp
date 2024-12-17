#include "Point.h"

Point::Point(LINE_TYPE type, float x, float y) : type(type), x(x), y(y)
{
}

void Point::Draw()
{
	Drawing::DrawPoint(this->x, this->y);
}
