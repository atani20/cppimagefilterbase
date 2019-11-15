#include "rect.h"


Rect::Rect(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {};
int Rect::CalcCoord(int coef, int size) {
	return (coef == 0) ? 0 : size / coef;
}
int Rect::getLeft() { return left; }
int Rect::getTop() { return top; }
int Rect::getRight() { return right; }
int Rect::getBottom() { return bottom; }
void Rect::transform(int h, int w) {
	left = CalcCoord(left, w);
	right = CalcCoord(right, w);
	top = CalcCoord(top, h);
	bottom = CalcCoord(bottom, h);
}