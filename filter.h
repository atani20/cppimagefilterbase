#pragma once
#include "png_toolkit.h"
#include "rect.h"

class Filter {
protected:
	image_data copy(image_data& imgData);
public:
	Filter() {};
	virtual void set(image_data& imgData, Rect &rect) = 0;
	virtual ~Filter() {};
};

class Red : public Filter {
public:
	Red() {};
	void set(image_data& imgData, Rect& rect) override;
};

class BlackAndWhite : public Filter {
public:
	BlackAndWhite() {};
	void set(image_data& imgData, Rect& rect) override;
};

class Threshold : public Filter {
private:
	int blockSize = 5;
	void setNewIntensity(image_data& imgData, image_data& imgOriginal, Rect& rect, int i0, int j0);
public:
	void set(image_data& imgData, Rect& rect) override;
};

class Blur : public Filter {
	int blockSize = 3;
	void blurPixel(image_data& imgData, image_data& imgOriginal, Rect& rect, int i0, int j0);
public:
	Blur() {};
	void set(image_data& imgData, Rect& rect) override;
};

class Edge : public Filter {
	int blockSize = 3;
	void edgePixel(image_data& imgData, image_data& imgOriginal, Rect& rect, int i0, int j0);
	int clump(int a);
public:
	Edge() {};
	void set(image_data& imgData, Rect& rect) override;
};