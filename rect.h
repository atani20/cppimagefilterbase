#pragma once

class Rect {
	int left, top, right, bottom;
	int CalcCoord(int coef, int size);
public:
	Rect(int l, int t, int r, int b);
	int getLeft();
	int getTop();
	int getRight();
	int getBottom();
	void transform(int h, int w);
};