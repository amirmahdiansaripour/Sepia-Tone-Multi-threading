#include "def.h"

void getPixlesFromBMP24(int end, char *fileReadBuffer, Image &image)
{
  int rows = image.pixcels.size();
  int cols = image.pixcels[0].size();
  int count = 1;
  int extra = cols % 4;
  for (int i = 0; i < rows; i++)
  {
    for (int j = cols - 1; j >= 0; j--){
      
      for (int k = 0; k < 3; k++)
      {
        switch (k)
        {
        case 0: // red 
          image.pixcels[i][j].red = fileReadBuffer[end - count++];
          break;
        case 1: // green
          image.pixcels[i][j].green = fileReadBuffer[end - count++];
          break;
        case 2: // blue
          image.pixcels[i][j].blue = fileReadBuffer[end - count++];
          break;
        }
      }
    }
  }
}

void writeOutBmp24(char *fileBuffer, const char *nameOfFileToCreate, int bufferSize, Image *image)
{
  int rows = image->pixcels.size();
  int cols = image->pixcels[0].size();
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
          fileBuffer[bufferSize - count++] = image->pixcels[i][j].red;
          // write red value in fileBuffer[bufferSize - count]
          break;
        case 1:
          fileBuffer[bufferSize - count++] = image->pixcels[i][j].green ;
          // write green value in fileBuffer[bufferSize - count]
          break;
        case 2:
          fileBuffer[bufferSize - count++] = image->pixcels[i][j].blue;
          // write blue value in fileBuffer[bufferSize - count]
          break;
        }
      }
    }
  }

  write.write(fileBuffer, bufferSize);
  write.close();
}

vector<int> fillAndAllocate(char *&buffer, const char *fileName, int &bufferSize)
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
    int rows = info_header->biHeight;
    int cols = info_header->biWidth;
    bufferSize = file_header->bfSize;
    file.close();
    return {rows, cols};
  }
  else
  {
    cout << "File" << fileName << " doesn't exist!" << endl;
    file.close();
    return {};
  }
}

