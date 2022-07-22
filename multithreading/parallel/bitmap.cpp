#include "def.h"

void getPixlesFromBMP24(int end, char *fileReadBuffer, Image &image){
  int rows = image.pixcels.size();
  int columns = image.pixcels[0].size();
  int count = 1;
  for (int i = 0; i < rows; i++){
    for (int j = columns - 1; j >= 0; j--){
      for (int k = 0; k < 3; k++){
        switch (k){
        case 0:
          image.pixcels[i][j].red = fileReadBuffer[end - count++];
          break;
        case 1:
          image.pixcels[i][j].green = fileReadBuffer[end - count++];
          break;
        case 2:
          image.pixcels[i][j].blue = fileReadBuffer[end - count++];
          break;
        }
      }
    }
  }
}

void writeOutBmp24(char *fileBuffer, string nameOfFileToCreate, int bufferSize, Image &image){
  int rows = image.pixcels.size();
  int columns = image.pixcels[0].size();
  ofstream write(nameOfFileToCreate);
  if (!write){
    cout << "Failed to write " << nameOfFileToCreate << endl;
    return;
  }
  int count = 1;
  for (int i = 0; i < rows; i++){
    for (int j = columns - 1; j >= 0; j--){
      for (int k = 0; k < 3; k++){
        switch (k){
        case 0:
          fileBuffer[bufferSize - count++] = image.pixcels[i][j].red;
          // write red value in fileBuffer
          break;
        case 1:
          fileBuffer[bufferSize - count++] = image.pixcels[i][j].green ;
          // write green value in fileBuffer
          break;
        case 2:
          fileBuffer[bufferSize - count++] = image.pixcels[i][j].blue;
          // write blue value in fileBuffer
          break;
        }
      }
    }
  }

  write.write(fileBuffer, bufferSize);
  write.close();
}

vector<int> getFileSize(char* buffer){
  PBITMAPFILEHEADER fileHeader;
  PBITMAPINFOHEADER infoHeader;

  fileHeader = (PBITMAPFILEHEADER)(&buffer[0]);
  infoHeader = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
  int rows = infoHeader->biHeight;
  int columns = infoHeader->biWidth;
  int bufferSize = fileHeader->bfSize; 
  return {rows, columns, bufferSize};
}

char* readBMP24(const char *fileName){
  ifstream file(fileName);

  if (file){
    file.seekg(0, ios::end);
    streampos length = file.tellg();
    file.seekg(0, ios::beg);

    char* buffer = new char[length];
    file.read(&buffer[0], length);

    file.close();
    return buffer;
  }
  else{
    cout << "File" << fileName << " doesn't exist!" << endl;
    file.close();
    exit(0);
  }
}

