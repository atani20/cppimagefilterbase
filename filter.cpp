#include <string>
#include <map>
#include <cstring>
#include <vector>
#include <algorithm>
#include "filter.h"

#include <iostream>
using namespace std;

image_data  Filter:: copy(image_data const& imgData) {
	image_data imgOriginal;
	int size = imgData.w * imgData.h * imgData.compPerPixel;
	imgOriginal.pixels = new stbi_uc[size];
	memcpy(imgOriginal.pixels, imgData.pixels,  size);
	return imgOriginal;
}
/*cout << endl << endl;
		for (int i = 0; i < imgData.h; i++) {
			for (int j = 0; j < imgData.w; j++)
				cout << (int)imgData.pixels[imgData.compPerPixel * (i * imgData.w + j)] << " ";
			cout << endl;
		}*/

void Red:: set(image_data& imgData, Rect &rect) {
	for (int i = rect.getTop(); i < rect.getBottom(); i++) {
		 for (int j = rect.getLeft(); j < rect.getRight(); j++){
			int pos = imgData.compPerPixel * (i * imgData.w + j);
			imgData.pixels[pos] = 255;
			imgData.pixels[pos + 1] = 0;
			imgData.pixels[pos + 2] = 0;
		}
	}
}


void BlackAndWhite::set(image_data& imgData, Rect &rect) {
	for (long i = rect.getTop(); i < rect.getBottom(); i++) {
		for (long j = rect.getLeft(); j < rect.getRight(); j++) {
			int pos = imgData.compPerPixel * (i * imgData.w + j);
			int intensity = (3 * imgData.pixels[pos] + 6 * imgData.pixels[pos + 1] + imgData.pixels[pos + 2])/10;
			imgData.pixels[pos] = intensity;
			imgData.pixels[pos + 1] = intensity;
			imgData.pixels[pos + 2] = intensity;
		}
	}
}


void Threshold::setNewIntensity(image_data& imgData, image_data& imgOriginal, Rect& rect, int const i0, int const j0) {
	int delta = blockSize / 2;
	vector <int> intensity;
	for (int i = i0 - delta; i <= i0 + delta; i++) {
		for (int j = j0 - delta; j <= j0 + delta; j++) {
			if (i < rect.getTop() || i >= rect.getBottom() || j < rect.getLeft() || j >= rect.getRight()) {
				continue;
			}
			int pos = imgData.compPerPixel * (i * imgData.w + j);
			intensity.push_back(imgOriginal.pixels[pos]);
		}
	}
	sort(intensity.begin(), intensity.end());
	int pos = imgData.compPerPixel * (i0 * imgData.w + j0);
	int newIntens = imgOriginal.pixels[pos] < intensity[intensity.size() / 2] ? 0 : imgOriginal.pixels[pos];
	imgData.pixels[pos] = newIntens;
	imgData.pixels[pos + 1] = newIntens;
	imgData.pixels[pos + 2] = newIntens;
}
void Threshold::set(image_data& imgData, Rect& rect) {
	BlackAndWhite bw;
	bw.set(imgData, rect);
	image_data imgOriginal = copy(imgData);
	for (int i = rect.getTop(); i < rect.getBottom(); i++) {
		for (int j = rect.getLeft(); j < rect.getRight(); j++) {
			setNewIntensity(imgData, imgOriginal, rect, i, j);
		}
	}
	delete[]imgOriginal.pixels;
}


void  Blur::blurPixel(image_data& imgData, image_data& imgOriginal, Rect& rect, int i0, int j0) {
	int sum[3] = { 0 };
	int delta = blockSize / 2;
	for (int i = i0 - delta; i <= i0 + delta; i++) {
		for (int j = j0 - delta; j <= j0 + delta; j++) {
			if (i < rect.getTop() || i >= rect.getBottom() || j < rect.getLeft() || j >= rect.getRight())
				continue;
			int pos = imgData.compPerPixel * (i * imgData.w + j);
			for (int k = 0; k < 3; k++)
				sum[k] += imgOriginal.pixels[pos + k];
		}
	}
	for (int k = 0; k < 3; k++)
		sum[k] /= 9;
	int pos = imgData.compPerPixel * (i0 * imgData.w + j0);
	for (int k = 0; k < 3; k++)
		imgData.pixels[pos + k] = sum[k];
}
void  Blur::set(image_data& imgData, Rect& rect) {
	image_data imgOriginal = copy(imgData);
	for (long i = rect.getTop(); i < rect.getBottom(); i++) {
		for (long j = rect.getLeft(); j < rect.getRight(); j++) {
			blurPixel(imgData, imgOriginal, rect, i, j);
		}
	}
	delete[]imgOriginal.pixels;
}

int Edge::clump(int num) {
	if (num < 0)
		return 0;
	else if (num > 255)
		return 255;
	return num;
}

void Edge::edgePixel(image_data& imgData, image_data const& imgOriginal, Rect & rect, int const i0, int const j0) {
	int sum = 0;
	int delta = blockSize / 2;
	for (int i = i0 - delta; i <= i0 + delta; i++) {
		for (int j = j0 - delta; j <= j0 + delta; j++) {
			if (i < rect.getTop() || i >= rect.getBottom() || j < rect.getLeft() || j >= rect.getRight())
				continue;
			int pos = imgData.compPerPixel * (i * imgData.w + j);
			if (i == i0 && j == j0) {
				sum += 9 * imgOriginal.pixels[pos];
			}
			else {
				sum -= imgOriginal.pixels[pos];
			}
		}
	}
	int color = clump(sum);
	int pos = imgData.compPerPixel * (i0 * imgData.w + j0);
	imgData.pixels[pos] = color;
	imgData.pixels[pos + 1] = color;
	imgData.pixels[pos + 2] = color;
}
void Edge::set(image_data& imgData, Rect& rect) {
	BlackAndWhite bw;
	bw.set(imgData, rect);
	image_data imgOriginal = copy(imgData);
	for (long i = rect.getTop(); i < rect.getBottom(); i++) {
		for (long j = rect.getLeft(); j < rect.getRight(); j++) {
			edgePixel(imgData, imgOriginal, rect, i, j);
		}
	}
	delete[]imgOriginal.pixels;
}
