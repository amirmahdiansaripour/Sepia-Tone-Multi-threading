#include "def.h"
vector<vector<Pixcel>> raw;

vector<PIGMENT> change_pixcels_blur(int row_range , int col_range){

  double sumRed = 0, sumBlue = 0, sumGreen = 0;
  
  for(int i = row_range - 1; i <= row_range + 1; i++)
    for(int j = col_range - 1; j <= col_range + 1; j++){
      sumRed += (double)(((int)raw[i][j].red) / 9);
      sumGreen += (double)(((int)raw[i][j].green) / 9);
      sumBlue += (double)(((int)raw[i][j].blue) / 9);
    }
  
    return {(PIGMENT) sumRed, (PIGMENT) sumGreen, (PIGMENT) sumBlue};
}

void blur(ImageThread &imageThread){

    int numberOfRows = imageThread.imagePointingTo->pixcels.size();
    int numberOfColumns = imageThread.imagePointingTo->pixcels[0].size(); 

    raw = vector<vector<Pixcel>>(numberOfRows, vector<Pixcel>(numberOfColumns));
        
    for(int i = 0; i < numberOfRows; i++)
        for(int j = 0; j < numberOfColumns; j++)
            raw[i][j] = imageThread.imagePointingTo->pixcels[i][j];

    int firstRow = (imageThread.firstRow == 0) ? 1 : imageThread.firstRow;  
    int lastRow = (imageThread.lastRow == raw.size() - 1) ? raw.size() - 2 : imageThread.lastRow;

    int firstColumn = (imageThread.firstColumn == 0) ? 1 : imageThread.firstColumn; 
    int lastColumn =  (imageThread.lastColumn == raw.size() - 1) ? raw.size() - 2 : imageThread.lastColumn;

    for(int i = firstRow ; i <= lastRow ; i++)
        for(int j = firstColumn ; j <= lastColumn; j++){
            vector<PIGMENT> results = change_pixcels_blur(i, j);
            imageThread.imagePointingTo->pixcels[i][j].red = results[0];
            imageThread.imagePointingTo->pixcels[i][j].green = results[1];
            imageThread.imagePointingTo->pixcels[i][j].blue = results[2];
        }
}
