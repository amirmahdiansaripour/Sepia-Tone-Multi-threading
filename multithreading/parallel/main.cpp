#include "def.h"

vector<vector<Pixcel>> raw;
pthread_t main_threads[4];
Image_thread image_threads[4];


// void change_pixcels_blur(int row_range , int col_range){
//   int start_x = row_range - 1;
//   int start_y = col_range - 1;
//   int end_x = row_range + 1;
//   int end_y = col_range + 1;
  
//   double sum_red = 0, sum_blue = 0, sum_green = 0;
  
//   for(int i = start_x; i <= end_x; i++)
//     for(int j = start_y; j <= end_y;j++){
//       sum_red += (double)(((int)raw[i][j].red) / 9);
//       sum_green += (double)(((int)raw[i][j].green) / 9);
//       sum_blue += (double)(((int)raw[i][j].blue) / 9);
//     }
  
//   Pixcels[row_range][col_range].red = (unsigned char)sum_red;
//   Pixcels[row_range][col_range].green = (unsigned char)sum_green;
//   Pixcels[row_range][col_range].blue = (unsigned char)sum_blue;
// }

// void sub_blur(int first_row, int last_row, int first_col , int last_col){
//   for(int i = first_row ; i <= last_row ; i++)
//     for(int j = first_col ; j <= last_col; j++)
//       change_pixcels_blur(i, j);
// }

// void blur(int first_r, int last_r, int first_c, int last_c){

//   raw = vector<vector<Pixcel>>(10, vector<Pixcel>(10));
  
//   for(int i = 0; i < 10; i++){
//     for(int j = 0; j < 10; j++){
//         raw[i][j] = Pixcels[i][j];
//     }
//   }
  
//   int first_row = (first_r == 0) ? 1 : first_r;  
//   int last_row = (last_r == raw.size() - 1) ? raw.size() - 2 : last_r;

//   int first_col = (first_c == 0) ? 1 : first_c; 
//   int last_col =  (last_c == raw.size() - 1) ? raw.size() - 2 : last_c;

//   sub_blur(first_row, last_row, first_col , last_col);
// }


void* thread_handler(void* threadId){
  long index = (long)threadId;
  int f_row = image_threads[index].first_row;
  int l_row = image_threads[index].last_row;
  int f_col = image_threads[index].first_col;
  int l_col = image_threads[index].last_col;
  // blur(f_row, l_row, f_col, l_col);
  sepia(f_row, l_row, f_col, l_col, *image_threads[index].imagePointingTo);
  pthread_exit(NULL);
}

void setThreadDimensions(int &rows, int &cols, Image* image){
  image_threads[0].first_row = 0; // first quarter
  image_threads[0].last_row = (rows - 1) / 2;
  image_threads[0].first_col = 0;
  image_threads[0].last_col = (cols - 1) / 2;
  image_threads[0].index = 0;
  
  image_threads[1].first_row = 0; // second
  image_threads[1].last_row = (rows - 1) / 2;
  image_threads[1].first_col = (cols + 1) / 2;
  image_threads[1].last_col = cols - 1;
  image_threads[1].index = 1;
  
  image_threads[2].first_row = (rows + 1) / 2;  // third
  image_threads[2].last_row = rows - 1;
  image_threads[2].first_col = (cols + 1) / 2;
  image_threads[2].last_col = cols - 1;
  image_threads[2].index = 2;
  
  image_threads[3].first_row = (rows + 1) / 2;  // forth
  image_threads[3].last_row = rows - 1;
  image_threads[3].first_col = 0;
  image_threads[3].last_col = (cols - 1) / 2;
  image_threads[3].index = 3;
  
  for(int i = 0; i < 4; i++){
    image_threads[i].imagePointingTo = new vector<vector<Pixcel>>(image->rows, vector<Pixcel>(image->cols));
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

  image->rows = dimensions[0];
  image->cols = dimensions[1];
  image->pixcels = vector<vector<Pixcel>>(image->rows, vector<Pixcel>(image->cols));
  
  getPixlesFromBMP24(bufferSize, fileBuffer, image);
  setThreadDimensions(image->rows, image->cols, image);
  handleThreads(image);
  writeOutBmp24(fileBuffer, "output.bmp", bufferSize, image, image->rows, image->cols);

//   washed_out(rows, cols, Pixcels);
 
//   cross(rows, cols, Pixcels);
  
// ////////////////////////////
  
//   gettimeofday(&enit, NULL);
//   r = (enit.tv_sec  + enit.tv_usec * 0.000001) -
//     (init.tv_sec  + init.tv_usec * 0.000001);
//   cout << "time consumed parallel: " << r << '\n';
  return 0;
}