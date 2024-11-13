// Created by Onkar Parag Kulkarni, Nov 2024
// Just for portfolio purposes and had some time between some project deadlines that's why xD

#pragma once
#include <vector>

struct sColor {
	float r, g, b, a;

	sColor();
	sColor(float r, float g, float b, float a);
	~sColor();
};

class cBMPImage {
public:
	cBMPImage(int width, int height);
	~cBMPImage();

	sColor GetColor(int x, int y);
	void SetColor(const sColor& color, int x, int y);

	void ExportBMP(const char* path) const;

private:
	int imageWidth;
	int imageHeight;
	std::vector<sColor> imageColors;
};