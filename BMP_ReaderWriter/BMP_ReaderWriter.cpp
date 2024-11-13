// Developed by Onkar Parag Kulkarni, Nov 2024
// Just for portfolio purposes and had some time between some project deadlines that's why xD

#include "pch.h"
#include "framework.h"
#include "../include/BMP_ReaderWriter.h"
#include <fstream>
#include <iostream>

sColor::sColor() : r(0.0f), g(0.0f), b(0.0f), a(0.0f)       // Sets every pixel color to black
{
}

sColor::sColor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)     // Sets pixel color to provided RGBA values
{
}

sColor::~sColor()
{
}

cBMPImage::cBMPImage(int width, int height) : imageWidth(width), imageHeight(height), imageColors(std::vector<sColor>(width * height))
{
    
}

cBMPImage::~cBMPImage()
{
}

sColor cBMPImage::GetColor(int x, int y) const
{
    return imageColors[y * imageWidth + x];
}

void cBMPImage::SetColor(const sColor& color, int x, int y)
{
    imageColors[y * imageWidth + x].r = color.r;
    imageColors[y * imageWidth + x].g = color.g;
    imageColors[y * imageWidth + x].b = color.b;
    imageColors[y * imageWidth + x].a = color.a;
}

void cBMPImage::ReadBMP(const char* path)
{
    std::ifstream inputFile;
    inputFile.open(path, std::ios::in | std::ios::binary);

    if (!inputFile.is_open()) {
        std::cerr << "File couldn't be opened!" << std::endl;
        return;
    }

    const int FILE_HEADER_SIZE = 14;
    const int INFO_HEADER_SIZE = 40;

    unsigned char fileHeader[FILE_HEADER_SIZE];
    unsigned char infoHeader[INFO_HEADER_SIZE];

    inputFile.read(reinterpret_cast<char*>(fileHeader), FILE_HEADER_SIZE);
    inputFile.read(reinterpret_cast<char*>(infoHeader), INFO_HEADER_SIZE);

    if (fileHeader[0] != 'B' || fileHeader[1] != 'M') {
        std::cerr << "Specified path is not .bmp image" << std::endl;
        inputFile.close();
        return;
    }

    int fileSize = fileHeader[2] 
        + (fileHeader[3] << 8) 
        + (fileHeader[4] << 16) 
        + (fileHeader[5] << 24);    // Reading from little endian

    imageWidth = infoHeader[4]
        + (infoHeader[5] << 8)
        + (infoHeader[6] << 16)
        + (infoHeader[7] << 24);

    imageHeight = infoHeader[8]
        + (infoHeader[9] << 8)
        + (infoHeader[10] << 16)
        + (infoHeader[11] << 24);

    imageColors.resize(imageWidth * imageHeight);

    // We don't need to calculate padding as we are using RGBA values

    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            unsigned char color[4];
            inputFile.read(reinterpret_cast<char*>(color), 4);

            imageColors[y * imageWidth + x].r = static_cast<float>(color[3]) / 255.0f;
            imageColors[y * imageWidth + x].g = static_cast<float>(color[2]) / 255.0f;
            imageColors[y * imageWidth + x].b = static_cast<float>(color[1]) / 255.0f;
            imageColors[y * imageWidth + x].a = static_cast<float>(color[0]) / 255.0f;
        }

        // Don't need padding here
    }
    inputFile.close();
    std::cout << "File read successfully!" << std::endl;
}

void cBMPImage::ExportBMP(const char* path) const
{
    std::ofstream outputFile;
    outputFile.open(path, std::ios::out | std::ios::binary);

    if (!outputFile.is_open()) {
        std::cerr << "File could not be opened!" << std::endl;
        return;
    }

    // IMP 3 parts I needed to know for writing this file
    // File Header              -       General Info --> Size 14
    // Information Header       -       Detailed Info --> Size 40
    // Color Data for each pixel
    // ------------------General Example for more clarity----------------------------
    // Image of width = 3 and height = 4
    // 3 bytes for red, 3 bytes for green, 3 bytes for blue and 3 bytes for alpha
    // |                        |                     |                    |  <-- Space we actually have
    // |~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|~~~~~~~~~~~~~~~~|  <-- Space required
    // |     Red        |      Green     |      Blue      |     Alpha      |
    // |                |                |                |                |
    // (In this case both are equal so we don't need padding)
    // (We would have required padding if we had used only rgb)
    // (values instead of rgba)

    const int FILE_HEADER_SIZE = 14;
    const int INFO_HEADER_SIZE = 40;
    const int FILE_SIZE = FILE_HEADER_SIZE + INFO_HEADER_SIZE + imageWidth * imageHeight * 4;       // We would have required padding in here if we had used only rgb values

    unsigned char fileHeader[FILE_HEADER_SIZE];
    unsigned char infoHeader[INFO_HEADER_SIZE];

    // File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';

    // File size
    fileHeader[2] = FILE_SIZE;
    fileHeader[3] = FILE_SIZE >> 8;
    fileHeader[4] = FILE_SIZE >> 16;
    fileHeader[5] = FILE_SIZE >> 24;

    // Reserved 1 (not using) 
    fileHeader[6] = 0;
    fileHeader[7] = 0;

    // Reserved 2 (not using) 
    fileHeader[8] = 0;
    fileHeader[9] = 0;

    // Pixel data offset
    fileHeader[10] = FILE_HEADER_SIZE + INFO_HEADER_SIZE;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    // Header size
    infoHeader[0] = INFO_HEADER_SIZE;
    infoHeader[1] = 0;
    infoHeader[2] = 0;
    infoHeader[3] = 0;

    // Image width  (Little Endian order for writing)
    infoHeader[4] = imageWidth;
    infoHeader[5] = imageWidth >> 8;
    infoHeader[6] = imageWidth >> 16;
    infoHeader[7] = imageWidth >> 24;

    // Image height
    infoHeader[8] = imageHeight;
    infoHeader[9] = imageHeight >> 8;
    infoHeader[10] = imageHeight >> 16;
    infoHeader[11] = imageHeight >> 24;

    // Planes
    infoHeader[12] = 1;     // I guess this is only one plane because it is 2D image
    infoHeader[13] = 0;

    // Bits per pixel RGBA
    infoHeader[14] = 32;
    infoHeader[15] = 0;

    // Compression (We have lot of space xD so won't use compression)
    infoHeader[16] = 0;
    infoHeader[17] = 0;
    infoHeader[18] = 0;
    infoHeader[19] = 0;
    
    // Image size (No compression)
    infoHeader[20] = 0;
    infoHeader[21] = 0;
    infoHeader[22] = 0;
    infoHeader[23] = 0;

    // X pixels per meter (not specified)
    infoHeader[24] = 0;
    infoHeader[25] = 0;
    infoHeader[26] = 0;
    infoHeader[27] = 0;

    // Y pixels per meter (not specified)
    infoHeader[28] = 0;
    infoHeader[29] = 0;
    infoHeader[30] = 0;
    infoHeader[31] = 0;

    // Total colors (Won't be using color pallets)
    infoHeader[32] = 0;
    infoHeader[33] = 0;
    infoHeader[34] = 0;
    infoHeader[35] = 0;

    // Important colors (generally ignored)
    infoHeader[36] = 0;
    infoHeader[37] = 0;
    infoHeader[38] = 0;
    infoHeader[39] = 0;

    // Almost done with writing just need to save
    outputFile.write(reinterpret_cast<char*>(fileHeader), FILE_HEADER_SIZE);
    outputFile.write(reinterpret_cast<char*>(infoHeader), INFO_HEADER_SIZE);

    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            unsigned char r = static_cast<unsigned char>(GetColor(x, y).r * 255.0f);
            unsigned char g = static_cast<unsigned char>(GetColor(x, y).g * 255.0f);
            unsigned char b = static_cast<unsigned char>(GetColor(x, y).b * 255.0f);
            unsigned char a = static_cast<unsigned char>(GetColor(x, y).a * 255.0f);

            unsigned char color[] = { a, b, g, r };

            outputFile.write(reinterpret_cast<char*>(color), 4);
        }

        // If we had used only rgb values then we would have required to write padding amount in here
    }

    outputFile.close();
    std::cout << "File created successfully!" << std::endl;
}