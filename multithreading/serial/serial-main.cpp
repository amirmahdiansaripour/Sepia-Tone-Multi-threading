#include "../filters/def.h"

void writeSerialTimeSpent(float timeSpent){
  fstream minValueRecord;
  minValueRecord.open(CSV_FILE, ios::out | ios::app);
  minValueRecord << timeSpent << '\n';
  minValueRecord.close();
}

float runSerial(Image* image, char *fileBuffer, int bufferSize){
    auto start = high_resolution_clock::now();
    sepia(*image);
    sepia(*image);
    sepia(*image);    
    writeOutBmp24(fileBuffer, OUTPUT_FOLDER_SERIAL, bufferSize, *image);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    return (float)duration.count();
}

int main(int argc, char *argv[]){
    char *fileName = argv[1];  
    char *fileBuffer = readBMP24(fileName); 
    vector<int> fileDimensions = getFileSize(fileBuffer);  
    int rows = fileDimensions[0];
    int columns = fileDimensions[1];
    int bufferSize = fileDimensions[2];
    Image* image = new Image;
    image->pixcels = vector<vector<Pixcel>>(rows, vector<Pixcel>(columns));
    getPixlesFromBMP24(bufferSize, fileBuffer, *image);
    float timeSpent = runSerial(image, fileBuffer, bufferSize);    
    
    writeSerialTimeSpent(timeSpent);
    free(image);
    free(fileBuffer);
    return 0;
}