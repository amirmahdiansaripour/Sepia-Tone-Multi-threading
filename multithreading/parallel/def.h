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
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using namespace std;
using namespace std::chrono;

typedef struct Pixcel{
  unsigned char green, blue, red;
  Pixcel(unsigned char g_, unsigned char r_, unsigned char b_){
    green = g_;
    red = r_;
    blue = b_;
  }
  Pixcel(){
    green = 0;
    red = 0;
    blue = 0;
  }
} Pixcel;

typedef struct Image_thread{
  int first_row;
  int first_col;
  int last_row;
  int last_col;
} Image_thread;

#pragma pack(1)
#pragma once

typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct tagBITMAPFILEHEADER
{
  WORD bfType;
  DWORD bfSize;
  WORD bfReserved1;
  WORD bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
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

typedef struct Image{
  vector<vector<Pixcel>> pixcels;
  int rows;
  int cols;
} Image;

vector<int> fillAndAllocate(char *&buffer, const char *fileName, int &bufferSize);
unsigned char calc_mean(int, int, const string&);
void blur(int first_r, int last_r, int first_c, int last_c);
void sepia(int first_row, int last_row, int first_col , int last_col , vector<vector<Pixcel>>& Pixcels);
void washed_out(int, int, vector<vector<Pixcel>>&);
void cross(int, int, vector<vector<Pixcel>>&);

#endif
