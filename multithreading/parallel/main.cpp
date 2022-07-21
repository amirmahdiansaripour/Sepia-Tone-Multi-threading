#include "def.h"

long NUMBER_OF_THREADS;
vector<ImageThread> imageThreads;

void* thread_handler(void* threadId){
  long index = (long)threadId;
  sepia(imageThreads[index]);
  sepia(imageThreads[index]);
  sepia(imageThreads[index]);
  pthread_exit(NULL);
}


void setThreadDimensions(Image* image){
  int rows = image->pixcels.size();
  int cols = image->pixcels[0].size();
  // cout << "rows\t" << rows << "\tcols\t" << cols << "\n"; 
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
    imageThreads[counter].lastColumn = cols - 1;
    imageThreads[counter].imagePointingTo = new Image;
    imageThreads[counter].imagePointingTo = image;
    imageThreads[counter].unprocessedImage = image->pixcels;
    counter++;
  }

  return;
}

void handleThreads(Image* image){
  int created, joined;
  for(long i = 0; i < NUMBER_OF_THREADS; i++){
    pthread_create(&imageThreads[i].thread, NULL, thread_handler, (void*)i);
  }
  for(long j = 0; j < NUMBER_OF_THREADS; j++){
    pthread_join(imageThreads[j].thread, NULL);
  }
}

vector<long> getDivedends(int imageSize){
  vector<long>dividends;
  for(int numOfthreads = 2; numOfthreads <= imageSize; numOfthreads++){
    if(imageSize % numOfthreads == 0)
      dividends.emplace_back(numOfthreads);
  }
  return dividends;
}

vector<string> runParallel(Image* image, char *fileBuffer, int bufferSize){
  float rate = (float) NUMBER_OF_THREADS / image->pixcels.size();
  auto start = high_resolution_clock::now();
  setThreadDimensions(image);
  handleThreads(image);
  writeOutBmp24(fileBuffer, "results/output" + to_string(NUMBER_OF_THREADS) + ".bmp", bufferSize, *image);
  auto end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(end - start);
  cout << "Time spent for: "  << NUMBER_OF_THREADS << "\t thread rate is "<< duration.count() << "\n";
  return {to_string(rate), to_string(duration.count())};
}

void writeToCSV(vector<vector<string>>& samples){
  ofstream file;
  file.open(CSV_FILE);
  file << "x,y\n";
  for(vector<string> row : samples){
    file << row[0] << "," << row[1] << "\n";
  }
  file.close();
}


int main(int argc, char *argv[]){

  char *fileName = argv[1];  
  char *temporary = readBMP24(fileName); 
  vector<int> fileDimensions = getFileSize(temporary);  
  int rows = fileDimensions[0];
  int cols = fileDimensions[1];
  int bufferSize = fileDimensions[2];

  vector<long> feasibleNumOfThreads = getDivedends(rows);
  vector<vector<string>> sampleOutput;
  for(int i = 0; i < feasibleNumOfThreads.size(); i++){
    char *fileBuffer = readBMP24(fileName);
    NUMBER_OF_THREADS = feasibleNumOfThreads[i];
    Image* image = new Image;
    imageThreads = vector<ImageThread>(NUMBER_OF_THREADS);
    image->pixcels = vector<vector<Pixcel>>(rows, vector<Pixcel>(cols));
    getPixlesFromBMP24(bufferSize, fileBuffer, *image);
    sampleOutput.emplace_back(runParallel(image, fileBuffer, bufferSize));
    free(image); 
    free(fileBuffer); 
  }
  writeToCSV(sampleOutput);
  free(temporary);
  return 0;
}