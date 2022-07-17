#ifndef _DEF_H__
#define _DEF_H_

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <vector>
#include <sys/time.h>
#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

#pragma pack(1)
#pragma once

typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct tagBITMAPFILEHEADER{
  WORD bfType;
  DWORD bfSize;
  WORD bfReserved1;
  WORD bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
  DWORD biSize;
  int biWidth;
  int biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  int biXPelsPerMeter;
  int biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef unsigned char PIGMENT;

typedef struct Pixcel{
  PIGMENT green, blue, red;
  Pixcel(PIGMENT g, PIGMENT r, PIGMENT b){
    green = g;
    red = r;
    blue = b;
  }
  Pixcel(){
    green = 0;
    red = 0;
    blue = 0;
  }
} Pixcel;

typedef struct ImageThread{
  int firstRow;
  int firstColumn;
  int lastRow;
  int lastColumn;
  int index;
  vector<vector<Pixcel>>* imagePointingTo;
} ImageThread;

typedef struct Image{
  vector<vector<Pixcel>> pixcels;
} Image;

void getPixlesFromBMP24(int end, char *fileReadBuffer, Image *image);
void writeOutBmp24(char *fileBuffer, const char *nameOfFileToCreate, int bufferSize, Image *image);
vector<int> fillAndAllocate(char *&buffer, const char *fileName, int &bufferSize);
PIGMENT calc_mean(int, int, const string&);
void blur(int first_r, int last_r, int first_c, int last_c, vector<vector<Pixcel>>& p);
void sepia(int firstRow, int lastRow, int firstColumn , int lastColumn , vector<vector<Pixcel>>& Pixcels);
void washed_out(int, int, vector<vector<Pixcel>>&);
void cross(int, int, vector<vector<Pixcel>>&);

#endif