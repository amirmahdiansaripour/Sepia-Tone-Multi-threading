#include "def.h"

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


