#ifndef _DEF_H__
#define _DEF_H_

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <sys/time.h>
#include <vector>
#include <bits/stdc++.h>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using namespace std;
using namespace std::chrono;

struct Pixcel{
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
};

typedef struct Pixcel Pixcel; 

unsigned char calc_mean(int, int, const string&);
void blur(int, int, vector<vector<Pixcel>>&);
void sepia(int, int, vector<vector<Pixcel>>&);
void washed_out(int, int, vector<vector<Pixcel>>&);
void cross(int, int, vector<vector<Pixcel>>&);

#endif