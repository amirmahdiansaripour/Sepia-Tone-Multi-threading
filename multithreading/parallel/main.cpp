#include "def.h"

pthread_t main_threads[4];
ImageThread image_threads[4];

void* thread_handler(void* threadId){
  long index = (long)threadId;
  int f_row = image_threads[index].firstRow;
  int l_row = image_threads[index].lastRow;
  int f_col = image_threads[index].firstColumn;
  int l_col = image_threads[index].lastColumn;
  blur(f_row, l_row, f_col, l_col, *image_threads[index].imagePointingTo);
  sepia(f_row, l_row, f_col, l_col, *image_threads[index].imagePointingTo);
  pthread_exit(NULL);
}

void setThreadDimensions(Image* image){
  int rows = image->pixcels.size();
  int cols = image->pixcels[0].size();
  image_threads[0].firstRow = 0; // first quarter
  image_threads[0].lastRow = (rows - 1) / 2;
  image_threads[0].firstColumn = 0;
  image_threads[0].lastColumn = (cols - 1) / 2;
  image_threads[0].index = 0;
  
  image_threads[1].firstRow = 0; // second
  image_threads[1].lastRow = (rows - 1) / 2;
  image_threads[1].firstColumn = (cols + 1) / 2;
  image_threads[1].lastColumn = cols - 1;
  image_threads[1].index = 1;
  
  image_threads[2].firstRow = (rows + 1) / 2;  // third
  image_threads[2].lastRow = rows - 1;
  image_threads[2].firstColumn = (cols + 1) / 2;
  image_threads[2].lastColumn = cols - 1;
  image_threads[2].index = 2;
  
  image_threads[3].firstRow = (rows + 1) / 2;  // forth
  image_threads[3].lastRow = rows - 1;
  image_threads[3].firstColumn = 0;
  image_threads[3].lastColumn = (cols - 1) / 2;
  image_threads[3].index = 3;
  
  for(int i = 0; i < 4; i++){
    image_threads[i].imagePointingTo = new vector<vector<Pixcel>>(image->pixcels.size(), 
    vector<Pixcel>(image->pixcels[0].size()));
    image_threads[i].imagePointingTo = &(image->pixcels);
  }

  return;
}

void handleThreads(Image* image){
    int create_result, join_result;

  for(long i = 0; i < 4; i++){
    create_result = pthread_create(&main_threads[i], NULL, thread_handler, (void*)i);
    if(create_result){
      cout << "Error on create\n";
      exit(-1);
    }
  }

  for(long i = 0; i < 4; i++){
    join_result = pthread_join(main_threads[i], NULL);
    if(join_result){
      cout << "Error on join\n";
      exit(-1);
    }
  }
}

int main(int argc, char *argv[])
{
  struct timeval init,enit;
  gettimeofday(&init, NULL);

  char *fileBuffer;
  int bufferSize;
  char *fileName = argv[1];
  Image* image = new Image;
  
  vector<int> dimensions = fillAndAllocate(fileBuffer, fileName, bufferSize); 
  if (dimensions.size() == 0)
  {
    cout << "File read error" << endl;
    return 1;
  }
  image->pixcels = vector<vector<Pixcel>>(dimensions[0], vector<Pixcel>(dimensions[1]));
  getPixlesFromBMP24(bufferSize, fileBuffer, image);
  setThreadDimensions(image);
  handleThreads(image);
  writeOutBmp24(fileBuffer, "output.bmp", bufferSize, image);

//   washed_out(rows, cols, Pixcels);
 
//   cross(rows, cols, Pixcels);
  
// ////////////////////////////
  
//   gettimeofday(&enit, NULL);
//   r = (enit.tv_sec  + enit.tv_usec * 0.000001) -
//     (init.tv_sec  + init.tv_usec * 0.000001);
//   cout << "time consumed parallel: " << r << '\n';
  return 0;
}