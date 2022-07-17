#include "def.h"

vector<ImageThread> imageThreads = vector<ImageThread>(NUMBER_OF_THREADS);

void* thread_handler(void* threadId){
  long index = (long)threadId;
  blur(imageThreads[index]);
  sepia(imageThreads[index]);
  pthread_exit(NULL);
}

void printDimensions(string str, int f_r, int l_r, int f_c, int l_c){
  cout << str << "\n";
  cout << "first row : " << f_r << "\n";
  cout << "last row : " << l_r << "\n";
  cout << "first column : " << f_c << "\n";
  cout << "last column : " << l_c << "\n";
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

int main(int argc, char *argv[]){
  char *fileBuffer;
  int bufferSize;
  char *fileName = argv[1];
  Image* image = new Image;
  
  vector<int> dimensions = fillAndAllocate(fileBuffer, fileName, bufferSize); 
  if (dimensions.size() == 0){
    cout << "File read error" << endl;
    return 1;
  }
  image->pixcels = vector<vector<Pixcel>>(dimensions[0], vector<Pixcel>(dimensions[1]));
  getPixlesFromBMP24(bufferSize, fileBuffer, *image);
  auto start = high_resolution_clock::now();
  setThreadDimensions(image);
  handleThreads(image);
  writeOutBmp24(fileBuffer, "output.bmp", bufferSize, *image);
  auto end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(end - start);
  cout << "Time spent parallel (ms): " << duration.count() << "\n";
  free(image);  
  return 0;
}