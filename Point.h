#pragma once
#include "Drawing.h"

enum LINE_TYPE {
	LINE,
	ARC
};

class Point
{
private:
	LINE_TYPE type;
	float x;
	float y;
public:
	Point* midPoint;
	Point(LINE_TYPE type, float x, float y);
	~Point() = default;
	void Draw();
	inline float GetX() { return this->x; }
	inline float GetY() { return this->y; }
	inline LINE_TYPE GetType() { return this->type; }
	inline void SetX(float x) { this->x = x; }
	inline void SetY(float y) { this->y = y; }
};

