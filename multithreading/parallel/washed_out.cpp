#include "def.h"

void setcolor(unsigned char& a, double mean){
    a = (unsigned char)(0.4 * a);
    if((int)((double)a + (0.6 * mean)) <= 255)
        a += (unsigned char)(0.6 * mean);
}

void washed_out(int rows, int cols, vector<vector<Pixcel>>& Pixcels){
    double mean_red = 0, mean_green = 0, mean_blue = 0;

    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++){
            mean_red += (double)(Pixcels[i][j].red);
            mean_green += (double)(Pixcels[i][j].green);
            mean_blue += (double)(Pixcels[i][j].blue);
        }
    
    mean_red = mean_red / (rows * cols);
    mean_green = mean_green / (rows * cols);
    mean_blue = mean_blue / (rows * cols);

    for(int i = 0; i < rows; i++)   
        for(int j = 0; j < cols; j++){
            setcolor(Pixcels[i][j].red, mean_red);
            setcolor(Pixcels[i][j].green, mean_green);
            setcolor(Pixcels[i][j].blue, mean_blue);
        } 
}