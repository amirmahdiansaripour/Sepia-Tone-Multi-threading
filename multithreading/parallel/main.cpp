#include "def.h"

int rows;
int cols;
vector<vector<Pixcel>> Pixcels;


void getPixlesFromBMP24(int end, int rows, int cols, char *fileReadBuffer)
{
  int count = 1;
  //int r_count = 0, b_count = 0, g_count = 0;
  int extra = cols % 4;
  for (int i = 0; i < rows; i++)
  {
    //count += extra;
    for (int j = cols - 1; j >= 0; j--){
      
      for (int k = 0; k < 3; k++)
      {
        switch (k)
        {
        case 0: // red 
          Pixcels[i][j].red = fileReadBuffer[end - count++];
          break;
        case 1: // green
          Pixcels[i][j].green = fileReadBuffer[end - count++];
          break;
        case 2: // blue
          Pixcels[i][j].blue = fileReadBuffer[end - count++];
          break;
        }
      }
    }
  }
}


void writeOutBmp24(char *fileBuffer, const char *nameOfFileToCreate, int bufferSize)
{
  std::ofstream write(nameOfFileToCreate);
  if (!write)
  {
    cout << "Failed to write " << nameOfFileToCreate << endl;
    return;
  }
  int count = 1;
  int extra = cols % 4;
  for (int i = 0; i < rows; i++)
  {
    //count += extra;
    for (int j = cols - 1; j >= 0; j--){
      for (int k = 0; k < 3; k++)
      {
        switch (k)
        {
        case 0:
          fileBuffer[bufferSize - count++] = Pixcels[i][j].red;
          // write red value in fileBuffer[bufferSize - count]
          break;
        case 1:
          fileBuffer[bufferSize - count++] = Pixcels[i][j].green ;
          // write green value in fileBuffer[bufferSize - count]
          break;
        case 2:
          fileBuffer[bufferSize - count++] = Pixcels[i][j].blue;
          // write blue value in fileBuffer[bufferSize - count]
          break;
        }
      }
    }
  }

  write.write(fileBuffer, bufferSize);
  write.close();
}

vector<vector<Pixcel>> raw;

void change_pixcels_blur(int row_range , int col_range){
  int start_x = row_range - 1;
  int start_y = col_range - 1;
  int end_x = row_range + 1;
  int end_y = col_range + 1;
  
  double sum_red = 0, sum_blue = 0, sum_green = 0;
  
  for(int i = start_x; i <= end_x; i++)
    for(int j = start_y; j <= end_y;j++){
      sum_red += (double)(((int)raw[i][j].red) / 9);
      sum_green += (double)(((int)raw[i][j].green) / 9);
      sum_blue += (double)(((int)raw[i][j].blue) / 9);
    }
  
  Pixcels[row_range][col_range].red = (unsigned char)sum_red;
  Pixcels[row_range][col_range].green = (unsigned char)sum_green;
  Pixcels[row_range][col_range].blue = (unsigned char)sum_blue;
}

void sub_blur(int first_row, int last_row, int first_col , int last_col){
  for(int i = first_row ; i <= last_row ; i++)
    for(int j = first_col ; j <= last_col; j++)
      change_pixcels_blur(i, j);
}

void blur(int first_r, int last_r, int first_c, int last_c){

  raw = vector<vector<Pixcel>>(rows, vector<Pixcel>(cols));
  
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
        raw[i][j] = Pixcels[i][j];
    }
  }
  
  int first_row = (first_r == 0) ? 1 : first_r;  
  int last_row = (last_r == raw.size() - 1) ? raw.size() - 2 : last_r;

  int first_col = (first_c == 0) ? 1 : first_c; 
  int last_col =  (last_c == raw.size() - 1) ? raw.size() - 2 : last_c;

  sub_blur(first_row, last_row, first_col , last_col);
}

pthread_t main_threads[4];
Image_thread image_threads[4];

void* thread_handler(void* tid){
  long index = (long)tid;
  int f_row = image_threads[index].first_row;
  int l_row = image_threads[index].last_row;
  int f_col = image_threads[index].first_col;
  int l_col = image_threads[index].last_col;
  blur(f_row, l_row, f_col, l_col);
  sepia(f_row, l_row, f_col, l_col, Pixcels);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  struct timeval init,enit;
  gettimeofday(&init, NULL);

  char *fileBuffer;
  int bufferSize;
  char *fileName = argv[1];
/////////////////////////////
  struct timeval start1,end1;
  gettimeofday(&start1, NULL);
  vector<int> dimensions = fillAndAllocate(fileBuffer, fileName, bufferSize); 
  if (dimensions.size() == 0)
  {
    cout << "File read error" << endl;
    return 1;
  }
  rows = dimensions[0];
  cols = dimensions[1];
  gettimeofday(&end1, NULL);
  double r = -(start1.tv_sec  + start1.tv_usec * 0.000001) +
    (end1.tv_sec  + end1.tv_usec * 0.000001);
  //cout << "duration of fillAndAllocate : " << r << "\n\n\n\n";
///////////////////////////  
  Pixcels = vector<vector<Pixcel>>(rows, vector<Pixcel>(cols));
///////////////////////////
  struct timeval start2,end2;
  gettimeofday(&start2, NULL);
  getPixlesFromBMP24(bufferSize, rows, cols, fileBuffer);
  gettimeofday(&end2, NULL);
  r = -(start2.tv_sec  + start2.tv_usec * 0.000001) +
    (end2.tv_sec  + end2.tv_usec * 0.000001);
  //cout << "duration of getPixcelFromBMP24 : " << r << "\n\n\n\n";
////////////////////////////


  struct timeval start3,end3;
  gettimeofday(&start3, NULL);

  image_threads[0].first_row = 0; // first quarter
  image_threads[0].last_row = (rows - 1) / 2;
  image_threads[0].first_col = 0;
  image_threads[0].last_col = (cols - 1) / 2;

  image_threads[1].first_row = 0; // second
  image_threads[1].last_row = (rows - 1) / 2;
  image_threads[1].first_col = (cols + 1) / 2;
  image_threads[1].last_col = cols - 1;

  image_threads[2].first_row = (rows + 1) / 2;  // third
  image_threads[2].last_row = rows - 1;
  image_threads[2].first_col = (cols + 1) / 2;
  image_threads[2].last_col = cols - 1;

  image_threads[3].first_row = (rows + 1) / 2;  // forth
  image_threads[3].last_row = rows - 1;
  image_threads[3].first_col = 0;
  image_threads[3].last_col = (cols - 1) / 2;

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
      cout << "Error on create\n";
      exit(-1);
    }
  }
  gettimeofday(&end3, NULL);
  r = -(start3.tv_sec  + start3.tv_usec * 0.000001) +
    (end3.tv_sec  + end3.tv_usec * 0.000001);
  
  //cout << "duration of blur and sepia: " << r << "\n\n\n\n";
////////////////////////////

  struct timeval start5,end5;
  gettimeofday(&start5, NULL);
  washed_out(rows, cols, Pixcels);
  gettimeofday(&end5, NULL);
  r = -(start5.tv_sec  + start5.tv_usec * 0.000001) +
    (end5.tv_sec  + end5.tv_usec * 0.000001);
  //cout << "duration of washed out : " << r << "\n\n\n\n";
////////////////////////////

 
  cross(rows, cols, Pixcels);
  
////////////////////////////

  struct timeval start7,end7;
  gettimeofday(&start7, NULL);
  writeOutBmp24(fileBuffer, "output.bmp", bufferSize);
  gettimeofday(&end7, NULL);
  r = -(start7.tv_sec  + start7.tv_usec * 0.000001) +
    (end7.tv_sec  + end7.tv_usec * 0.000001);
  
  //cout << "duration of writeOutBmp24 : " << r << "\n\n\n\n";
////////////////////////////
  
  gettimeofday(&enit, NULL);
  r = (enit.tv_sec  + enit.tv_usec * 0.000001) -
    (init.tv_sec  + init.tv_usec * 0.000001);
  cout << "time consumed parallel: " << r << '\n';
  return 0;
}