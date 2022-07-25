#include "../filters/def.h"

float runSerial(Image* image, char *fileBuffer, int bufferSize){
    auto start = high_resolution_clock::now();
    setThreadDimensions(image);
    handleThreads(image);
    writeOutBmp24(fileBuffer, OUTPUT_FOLDER_PARALLEL + to_string(NUMBER_OF_THREADS) + "threads.bmp", bufferSize, *image);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    // cout << "Time spent for: "  << NUMBER_OF_THREADS << "\t threads is "<< duration.count() << "\n";
    return {(float) NUMBER_OF_THREADS, (float)duration.count()};
}

int main(int argc, char *argv[]){
    char *fileName = argv[1];  
    char* runSingleOrHundred = argv[2];
    char *fileBuffer = readBMP24(fileName); 
    vector<int> fileDimensions = getFileSize(fileBuffer);  
    int rows = fileDimensions[0];
    int columns = fileDimensions[1];
    int bufferSize = fileDimensions[2];
    Image* image = new Image;
    image->pixcels = vector<vector<Pixcel>>(rows, vector<Pixcel>(columns));
    getPixlesFromBMP24(bufferSize, fileBuffer, *image);
    runSerial(image, fileBuffer, bufferSize);    

    free(image);
    free(fileBuffer);

  vector<long> feasibleNumOfThreads = getDivedends(rows);
  vector<vector<float>> sampleOutput;
  for(int i = 0; i < feasibleNumOfThreads.size(); i++){
    image->pixcels = vector<vector<Pixcel>>(rows, vector<Pixcel>(columns));
    getPixlesFromBMP24(bufferSize, fileBuffer, *image);
    sampleOutput.emplace_back(runParallel(image, fileBuffer, bufferSize));
    free(image); 
    free(fileBuffer); 
  }
  writeToCSV(runSingleOrHundred, sampleOutput);
  free(temporary);
  return 0;
}