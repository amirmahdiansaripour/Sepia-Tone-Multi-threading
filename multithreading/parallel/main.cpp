#include "def.h"

long NUMBER_OF_THREADS;
vector<ImageThread> imageThreads;

void* thread_handler(void* threadId){
  long index = (long)threadId;
  blur(imageThreads[index]);
  sepia(imageThreads[index]);
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
    imageThreads[counter].lastColumn = 399;
    imageThreads[counter].imagePointingTo = new Image;
    imageThreads[counter].imagePointingTo = image;
    counter++;
  }

  return;
}

void handleThreads(Image* image){
  int created, joined;
  for(long i = 0; i < NUMBER_OF_THREADS; i++){
    created = pthread_create(&imageThreads[i].thread, NULL, thread_handler, (void*)i);
    if(created){
      cout << "Error on create\n";
      exit(-1);
    }
  }

  for(long i = 0; i < NUMBER_OF_THREADS; i++){
    joined = pthread_join(imageThreads[i].thread, NULL);
    if(joined){
      cout << "Error on join\n";
      exit(-1);
    }
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

void runParallel(Image* image, char *fileBuffer, int bufferSize, long dividend){
  auto start = high_resolution_clock::now();
  setThreadDimensions(image);
  handleThreads(image);
  writeOutBmp24(fileBuffer, "output2.bmp", bufferSize, *image);
  auto end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(end - start);
  cout << "Time spent for: "  << NUMBER_OF_THREADS << "threads is "<< duration.count() << "\n";
}


int main(int argc, char *argv[]){
  char *fileBuffer;
  int bufferSize;
  char *fileName = argv[1];
  vector<int> dimensions = fillAndAllocate(fileBuffer, fileName, bufferSize); 
  if (dimensions.size() == 0){
    cout << "File read error" << endl;
    return 1;
  }
  vector<long> feasibleNumOfThreads = {8};
  for(long dividend : feasibleNumOfThreads){
    NUMBER_OF_THREADS = dividend;
    Image* image = new Image;
    imageThreads = vector<ImageThread>(NUMBER_OF_THREADS);
    image->pixcels = vector<vector<Pixcel>>(dimensions[0], vector<Pixcel>(dimensions[1]));
    getPixlesFromBMP24(bufferSize, fileBuffer, *image);
    runParallel(image, fileBuffer, bufferSize, dividend);
    free(image);  
  }
  return 0;
}