#include "rect.h"


Rect::Rect(int t, int l,  int b, int r) : top(t),left(l), bottom(b), right(r) {};
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