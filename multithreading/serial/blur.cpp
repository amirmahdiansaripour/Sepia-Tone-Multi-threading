
#include "def.h"
vector<vector<Pixcel>> raw;

void change_pixcels_blur(int row_range , int col_range, vector<vector<Pixcel>>& Pixcels){
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
  
  Pixcels[row_range][col_range].red = (PIGMENT)sum_red;
  Pixcels[row_range][col_range].green = (PIGMENT)sum_green;
  Pixcels[row_range][col_range].blue = (PIGMENT)sum_blue;
}

void sub_blur(int rows, int cols, vector<vector<Pixcel>>& Pixcels){
  for(int i = 1; i < rows - 1; i++)
    for(int j = 1; j < cols - 1; j++)
      change_pixcels_blur(i, j, Pixcels);
}

void blur(int rows, int cols, vector<vector<Pixcel>>& Pixcels){
  raw = vector<vector<Pixcel>>(rows, vector<Pixcel>(cols));
  
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
        raw[i][j] = Pixcels[i][j];
    }
  }

  sub_blur(rows, cols, Pixcels);

}

