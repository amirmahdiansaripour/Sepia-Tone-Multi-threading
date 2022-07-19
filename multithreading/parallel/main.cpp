#include "def.h"

long NUMBER_OF_THREADS;
vector<ImageThread> imageThreads;

void* thread_handler(void* threadId){
  long index = (long)threadId;
  blur(imageThreads[index]);
  blur(imageThreads[index]);
  blur(imageThreads[index]);
  pthread_exit(NULL);
}


void setThreadDimensions(Image* image){
  int rows = image->pixcels.size();
  int cols = image->pixcels[0].size();
  
  if(rows % NUMBER_OF_THREADS != 0){
    cout << "Can not divide image to equally squared segments\n";
    exit(0);
  }
  int offset = rows / NUMBER_OF_THREADS;
  int counter = 0;
  for(int i = 0; i <= rows - 1; i += offset){
    imageThreads[counter].firstRow = i;
    imageThreads[counter].lastRow = i + offset;
    imageThreads[counter].firstColumn = 0;
    imageThreads[counter].lastColumn = image->pixcels.size();
    imageThreads[counter].imagePointingTo = new Image;
    imageThreads[counter].imagePointingTo = image;
    counter++;
  }

  return;
}

void handleThreads(Image* image){
  int created, joined;
  for(long i = 0; i < NUMBER_OF_THREADS; i++){
    pthread_create(&imageThreads[i].thread, NULL, thread_handler, (void*)i);
    pthread_join(imageThreads[i].thread, NULL);
  }
}

vector<long> getDivedends(int divisor){
  vector<long>dividends;
  for(int i = 2; i <= divisor; i++){
    if(divisor % i == 0)
      dividends.emplace_back(i);
  }
  return dividends;
}

void runParallel(Image* image, char *fileBuffer, int bufferSize, float rate){
  auto start = high_resolution_clock::now();
  setThreadDimensions(image);
  handleThreads(image);
  writeOutBmp24(fileBuffer, "results/output" + to_string(rate) + ".bmp", bufferSize, *image);
  auto end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(end - start);
  cout << "Time spent for: "  << rate << "\t thread rate is "<< duration.count() << "\n";
}

int main(int argc, char *argv[]){

  char *fileName = argv[1];  
  char *temporary = readBMP24(fileName); 
  vector<int> fileDimensions = getFileSize(temporary);  
  int rows = fileDimensions[0];
  int cols = fileDimensions[1];
  int bufferSize = fileDimensions[2];

  vector<long> feasibleNumOfThreads = getDivedends(rows);

  for(int i = 0; i < feasibleNumOfThreads.size(); i++){
    float threadRate = (float) feasibleNumOfThreads[i] / rows;
    char *fileBuffer = readBMP24(fileName);
    NUMBER_OF_THREADS = feasibleNumOfThreads[i];
    Image* image = new Image;
    imageThreads = vector<ImageThread>(NUMBER_OF_THREADS);
    image->pixcels = vector<vector<Pixcel>>(rows, vector<Pixcel>(cols));
    getPixlesFromBMP24(bufferSize, fileBuffer, *image);
    runParallel(image, fileBuffer, bufferSize, threadRate);
    free(image); 
    free(fileBuffer); 
  }
  free(temporary);
  return 0;
}