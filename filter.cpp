#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "filter.h"

#include <iostream>
using namespace std;

image_data  Filter:: copy(image_data& imgData) {
	image_data imgOriginal;
	int size = imgData.w * imgData.h * imgData.compPerPixel;
	imgOriginal.pixels = new stbi_uc[size];
	memcpy(imgOriginal.pixels, imgData.pixels,  size);
	return imgOriginal;
}

void Red::set(image_data& imgData, Rect& rect) {
	for (long i = rect.getTop(); i < rect.getBottom(); i++) {
		for (long j = rect.getLeft(); j < rect.getRight(); j++) {
			int pos = imgData.compPerPixel * (i * imgData.w + j);
			imgData.pixels[pos] = 255;
			imgData.pixels[pos + 1] = 0;
			imgData.pixels[pos + 2] = 0;
		}
	}
}


void BlackAndWhite::set(image_data& imgData, Rect& rect) {
	for (long i = rect.getTop(); i < rect.getBottom(); i++) {
		for (long j = rect.getLeft(); j < rect.getRight(); j++) {
			int pos = imgData.compPerPixel * (i * imgData.w + j);
			int intensity = (3 * imgData.pixels[pos] + 6 * imgData.pixels[pos + 1] + imgData.pixels[pos + 2]) / 10;
			imgData.pixels[pos] = intensity;
			imgData.pixels[pos + 1] = intensity;
			imgData.pixels[pos + 2] = intensity;
		}
	}
}


void Threshold::setNewIntensity(image_data& imgData, Rect& rect, int i0, int j0) {
	vector <stbi_uc*> arrIntensity;
	for (int i = i0; i < i0 + blockSize; i++) {
		for (int j = j0; j < j0 + blockSize; j++) {
			if (i >= rect.getBottom() || j >= rect.getRight())
				continue;
			else {
				int pos = imgData.compPerPixel * (i * imgData.w + j);
				arrIntensity.push_back(imgData.pixels + pos);
			}
		}
	}
	sort(arrIntensity.begin(), arrIntensity.end(), [](stbi_uc* a, stbi_uc* b) ->bool
	{
		return *a < *b;
	});
	for (int i = 0; i < arrIntensity.size() / 2; i++) {
		*arrIntensity[i] = 0;
		*(arrIntensity[i] + 1) = 0;
		*(arrIntensity[i] + 2) = 0;
	}
}
void Threshold::set(image_data& imgData, Rect& rect) {
	BlackAndWhite bw;
	bw.set(imgData, rect);
	for (int i = rect.getTop(); i < rect.getBottom(); i += blockSize) {
		for (int j = rect.getLeft(); j < rect.getRight(); j += blockSize) {
			setNewIntensity(imgData, rect, i, j);
		}
	}
}


void  Blur::blurPixel(image_data& imgData, image_data& imgOriginal, Rect& rect, int i0, int j0) {
	int sum[3] = { 0 };
	for (int i = i0 - 1; i <= i0 + 1; i++) {
		for (int j = j0 - 1; j <= j0 + 1; j++) {
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

void Edge::edgePixel(image_data& imgData, image_data& imgOriginal, Rect& rect, int i0, int j0) {
	int sum = 0;
	for (int i = i0 - 1; i <= i0 + 1; i++) {
		for (int j = j0 - 1; j <= j0 + 1; j++) {
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
	sum /= 9;
	int pos = imgData.compPerPixel * (i0 * imgData.w + j0);
	imgData.pixels[pos] = sum;
	imgData.pixels[pos + 1] = sum;
	imgData.pixels[pos + 2] = sum;
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