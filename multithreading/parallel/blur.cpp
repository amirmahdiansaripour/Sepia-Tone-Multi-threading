#include "def.h"
vector<vector<Pixcel>> raw;

vector<PIGMENT> change_pixcels_blur(int row_range , int col_range){
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
  
    return {(PIGMENT) sum_red, (PIGMENT) sum_green, (PIGMENT) sum_blue};
}

void blur(int first_r, int last_r, int first_c, int last_c, vector<vector<Pixcel>>& Pixcels){

  raw = vector<vector<Pixcel>>(Pixcels.size(), vector<Pixcel>(Pixcels[0].size()));
  
  for(int i = 0; i < Pixcels.size(); i++){
    for(int j = 0; j < Pixcels[0].size(); j++){
        raw[i][j] = Pixcels[i][j];
    }
  }
  
  int firstRow = (first_r == 0) ? 1 : first_r;  
  int lastRow = (last_r == raw.size() - 1) ? raw.size() - 2 : last_r;

  int firstColumn = (first_c == 0) ? 1 : first_c; 
  int lastColumn =  (last_c == raw.size() - 1) ? raw.size() - 2 : last_c;

  for(int i = firstRow ; i <= lastRow ; i++)
    for(int j = firstColumn ; j <= lastColumn; j++){
        vector<PIGMENT> results = change_pixcels_blur(i, j);
        Pixcels[i][j].red = results[0];
        Pixcels[i][j].green = results[1];
        Pixcels[i][j].blue = results[2];
    }
}
