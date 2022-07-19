#ifndef _DEF_H__
#define _DEF_H__

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <vector>
#include <sys/time.h>
#include <bits/stdc++.h>
#include <pthread.h>
#include <math.h>
using namespace std;
using namespace std::chrono;

#pragma pack(1)
#pragma once
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned char PIGMENT;

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

typedef struct Image{
  vector<vector<Pixcel>> pixcels;
} Image;

typedef struct ImageThread{
  int firstRow;
  int firstColumn;
  int lastRow;
  int lastColumn;
  pthread_t thread;
  Image* imagePointingTo;
} ImageThread;

char* readBMP24(const char *);
vector<int> getFileSize(char* buffer);
void getPixlesFromBMP24(int, char*, Image&);
void writeOutBmp24(char*, string, int, Image&);
void blur(ImageThread&);
void sepia(ImageThread&);
#endif