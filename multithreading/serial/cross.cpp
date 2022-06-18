#include "def.h"

void cross_forward(vector<int>& A, int rows, int cols, vector<vector<Pixcel>>& Pixcels){
    int x = A[0];
    int y = A[1];
    while(x < rows && y < cols){
        Pixcels[x][y].red = 255;
        Pixcels[x][y].blue = 255;
        Pixcels[x][y].green = 255;
        x++; y++;
    }
}

void cross_backward(vector<int>& A, int rows, int cols, vector<vector<Pixcel>>& Pixcels){
    int x = A[0];
    int y = A[1];
    while(x < rows && y >= 0){
        Pixcels[x][y].red = 255;
        Pixcels[x][y].blue = 255;
        Pixcels[x][y].green = 255;
        x++; y--;
    }
}

void cross(int rows, int cols, vector<vector<Pixcel>>& Pixcels){
    vector<vector<int>> A(6, vector<int>(2));
    A[0] = {0, 0};
    A[1] = {0, 1};
    A[2] = {1, 0};
    A[3] = {0, cols - 1};
    A[4] = {0, cols - 2};
    A[5] = {1, cols - 1};
    cross_forward(A[0], rows, cols, Pixcels);
    cross_forward(A[1], rows, cols, Pixcels);
    cross_forward(A[2], rows, cols, Pixcels);
    cross_backward(A[3], rows, cols, Pixcels);
    cross_backward(A[4], rows, cols, Pixcels);
    cross_backward(A[5], rows, cols, Pixcels);
    
}