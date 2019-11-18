#pragma once

class Rect {
	int top, left, bottom, right;
	int CalcCoord(int coef, int size);
public:
	Rect(int t, int l, int b, int r);
	int getLeft();
	int getTop();
	int getRight();
	int getBottom();
	void transform(int h, int w);
};