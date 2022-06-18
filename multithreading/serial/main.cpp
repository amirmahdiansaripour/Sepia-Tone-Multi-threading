#include "def.h"

#pragma pack(1)
#pragma once


typedef int LONG;
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
  LONG biWidth;
  LONG biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG biXPelsPerMeter;
  LONG biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

int rows;
int cols;
vector<vector<Pixcel>> Pixcels;

bool fillAndAllocate(char *&buffer, const char *fileName, int &rows, int &cols, int &bufferSize)
{
  std::ifstream file(fileName);

  if (file)
  {
    file.seekg(0, std::ios::end);
    std::streampos length = file.tellg();
    file.seekg(0, std::ios::beg);



    buffer = new char[length];
    file.read(&buffer[0], length);

    PBITMAPFILEHEADER file_header;
    PBITMAPINFOHEADER info_header;

    file_header = (PBITMAPFILEHEADER)(&buffer[0]);
    info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
    rows = info_header->biHeight;
    cols = info_header->biWidth;
    bufferSize = file_header->bfSize;
    file.close();
    return 1;
  }
  else
  {
    cout << "File" << fileName << " doesn't exist!" << endl;
    file.close();
    return 0;
  }
}





void getPixlesFromBMP24(int end, int rows, int cols, char *fileReadBuffer)
{
  int count = 1;
  //int r_count = 0, b_count = 0, g_count = 0;
  int extra = cols % 4;
  for (int i = 0; i < rows; i++)
  {
    //count += extra;
    for (int j = cols - 1; j >= 0; j--){
      
      for (int k = 0; k < 3; k++)
      {
        switch (k)
        {
        case 0: // red 
          Pixcels[i][j].red = fileReadBuffer[end - count++];
          break;
        case 1: // green
          Pixcels[i][j].green = fileReadBuffer[end - count++];
          break;
        case 2: // blue
          Pixcels[i][j].blue = fileReadBuffer[end - count++];
          break;
        }
      }
    }
  }
}

void writeOutBmp24(char *fileBuffer, const char *nameOfFileToCreate, int bufferSize)
{
  std::ofstream write(nameOfFileToCreate);
  if (!write)
  {
    cout << "Failed to write " << nameOfFileToCreate << endl;
    return;
  }
  int count = 1;
  int extra = cols % 4;
  for (int i = 0; i < rows; i++)
  {
    //count += extra;
    for (int j = cols - 1; j >= 0; j--){
      for (int k = 0; k < 3; k++)
      {
        switch (k)
        {
        case 0:
          fileBuffer[bufferSize - count++] = Pixcels[i][j].red;
          // write red value in fileBuffer[bufferSize - count]
          break;
        case 1:
          fileBuffer[bufferSize - count++] = Pixcels[i][j].green ;
          // write green value in fileBuffer[bufferSize - count]
          break;
        case 2:
          fileBuffer[bufferSize - count++] = Pixcels[i][j].blue;
          // write blue value in fileBuffer[bufferSize - count]
          break;
        }
      }
    }
  }

  write.write(fileBuffer, bufferSize);
  write.close();
}

int main(int argc, char *argv[])
{
  struct timeval init,enit;
  gettimeofday(&init, NULL);

  char *fileBuffer;
  int bufferSize;
  char *fileName = argv[1];
/////////////////////////////
  struct timeval start1,end1;
  gettimeofday(&start1, NULL);
  if (!fillAndAllocate(fileBuffer, fileName, rows, cols, bufferSize))
  {
    cout << "File read error" << endl;
    return 1;
  }
  gettimeofday(&end1, NULL);
  double r = -(start1.tv_sec  + start1.tv_usec * 0.000001) +
    (end1.tv_sec  + end1.tv_usec * 0.000001);
  //cout << "duration of fillAndAllocate : " << r << "\n\n\n\n";
///////////////////////////  
  Pixcels = vector<vector<Pixcel>>(rows, vector<Pixcel>(cols));
///////////////////////////

  struct timeval start2,end2;
  gettimeofday(&start2, NULL);
  getPixlesFromBMP24(bufferSize, rows, cols, fileBuffer);
  gettimeofday(&end2, NULL);
  r = -(start2.tv_sec  + start2.tv_usec * 0.000001) +
    (end2.tv_sec  + end2.tv_usec * 0.000001);
  //cout << "duration of getPixcelFromBMP24 : " << r << "\n\n\n\n";
////////////////////////////

  struct timeval start3,end3;
  gettimeofday(&start3, NULL);
  blur(rows, cols, Pixcels);
  gettimeofday(&end3, NULL);
  r = -(start3.tv_sec  + start3.tv_usec * 0.000001) +
    (end3.tv_sec  + end3.tv_usec * 0.000001);
  
  //cout << "duration of blur : " << r << "\n\n\n\n";
////////////////////////////

  struct timeval start4,end4;
  gettimeofday(&start4, NULL);
  sepia(rows, cols, Pixcels);
  gettimeofday(&end4, NULL);
  r = -(start4.tv_sec  + start4.tv_usec * 0.000001) +
    (end4.tv_sec  + end4.tv_usec * 0.000001);
  //cout << "duration of sepia : " << r << "\n\n\n\n";
////////////////////////////

  struct timeval start5,end5;
  gettimeofday(&start5, NULL);
  washed_out(rows, cols, Pixcels);
  gettimeofday(&end5, NULL);
  r = -(start5.tv_sec  + start5.tv_usec * 0.000001) +
    (end5.tv_sec  + end5.tv_usec * 0.000001);
  //cout << "duration of washed out : " << r << "\n\n\n\n";
////////////////////////////


  cross(rows, cols, Pixcels);
  
////////////////////////////

  struct timeval start7,end7;
  gettimeofday(&start7, NULL);
  writeOutBmp24(fileBuffer, "output.bmp", bufferSize);
  gettimeofday(&end7, NULL);
  r = -(start7.tv_sec  + start7.tv_usec * 0.000001) +
    (end7.tv_sec  + end7.tv_usec * 0.000001);
  
  //cout << "duration of writeOutBmp24 : " << r << "\n\n\n\n";
////////////////////////////

  gettimeofday(&enit, NULL);
  r = (enit.tv_sec  + enit.tv_usec * 0.000001) -
    (init.tv_sec  + init.tv_usec * 0.000001);
  cout << "time consumed serial: " << r << '\n';
  return 0;
}