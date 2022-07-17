#include "def.h"

ImageThread imageThreads[4];

void* thread_handler(void* threadId){
  long index = (long)threadId;
  int firstRow = imageThreads[index].firstRow;
  int lastRow = imageThreads[index].lastRow;
  int firstColumn = imageThreads[index].firstColumn;
  int lastColumn = imageThreads[index].lastColumn;
  blur(firstRow, lastRow, firstColumn, lastColumn, *imageThreads[index].imagePointingTo);
  sepia(firstRow, lastRow, firstColumn, lastColumn, *imageThreads[index].imagePointingTo);
  pthread_exit(NULL);
}

void setThreadDimensions(Image* image){
  int rows = image->pixcels.size();
  int cols = image->pixcels[0].size();
  imageThreads[0].firstRow = 0; // first quarter
  imageThreads[0].lastRow = (rows - 1) / 2;
  imageThreads[0].firstColumn = 0;
  imageThreads[0].lastColumn = (cols - 1) / 2;
  imageThreads[0].index = 0;
  
  imageThreads[1].firstRow = 0; // second
  imageThreads[1].lastRow = (rows - 1) / 2;
  imageThreads[1].firstColumn = (cols + 1) / 2;
  imageThreads[1].lastColumn = cols - 1;
  imageThreads[1].index = 1;
  
  imageThreads[2].firstRow = (rows + 1) / 2;  // third
  imageThreads[2].lastRow = rows - 1;
  imageThreads[2].firstColumn = (cols + 1) / 2;
  imageThreads[2].lastColumn = cols - 1;
  imageThreads[2].index = 2;
  
  imageThreads[3].firstRow = (rows + 1) / 2;  // forth
  imageThreads[3].lastRow = rows - 1;
  imageThreads[3].firstColumn = 0;
  imageThreads[3].lastColumn = (cols - 1) / 2;
  imageThreads[3].index = 3;
  
  for(int i = 0; i < 4; i++){
    imageThreads[i].imagePointingTo = new vector<vector<Pixcel>>(image->pixcels.size(), 
    vector<Pixcel>(image->pixcels[0].size()));
    imageThreads[i].imagePointingTo = &(image->pixcels);
  }

  return;
}

void handleThreads(Image* image){
  int created, joined;
  for(long i = 0; i < 4; i++){
    created = pthread_create(&imageThreads[i].thread, NULL, thread_handler, (void*)i);
    if(created){
      cout << "Error on create\n";
      exit(-1);
    }
  }

  for(long i = 0; i < 4; i++){
    joined = pthread_join(imageThreads[i].thread, NULL);
    if(joined){
      cout << "Error on join\n";
      exit(-1);
    }
  }
}

int main(int argc, char *argv[]){
  struct timeval init,enit;
  gettimeofday(&init, NULL);

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
  getPixlesFromBMP24(bufferSize, fileBuffer, image);
  setThreadDimensions(image);
  handleThreads(image);
  writeOutBmp24(fileBuffer, "output.bmp", bufferSize, image);

//   washed_out(rows, cols, Pixcels);
  
//   gettimeofday(&enit, NULL);
//   r = (enit.tv_sec  + enit.tv_usec * 0.000001) -
//     (init.tv_sec  + init.tv_usec * 0.000001);
//   cout << "time consumed parallel: " << r << '\n';
  return 0;
}