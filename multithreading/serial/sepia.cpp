#include "def.h"

void set_color(unsigned char& a, vector<double>& values){
    a = (unsigned char)values[2];

    if((int)(a + (unsigned char)values[1]) < 255)
        a += (unsigned char)values[1];
    else{
        a = 255;
        return;
    }
        
    
    if((int)(a + (unsigned char)values[0]) < 255)
        a += (unsigned char)values[0];
    else
        a = 255;
       
}

void sepia(int rows, int cols, vector<vector<Pixcel>>& Pixcels){
    
    vector<double> values1, values2, values3;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            values1 = {(Pixcels[i][j].red * 0.393), (Pixcels[i][j].green * 0.769), (Pixcels[i][j].blue * 0.189)};  
            sort(values1.begin(), values1.end());
            values2 = {(Pixcels[i][j].red * 0.349), (Pixcels[i][j].green * 0.686), (Pixcels[i][j].blue * 0.168)};
            sort(values2.begin(), values2.end());
            values3 = {(Pixcels[i][j].red * 0.272), (Pixcels[i][j].green * 0.534), (Pixcels[i][j].blue * 0.131)};
            sort(values3.begin(), values3.end());
            

            set_color(Pixcels[i][j].red, values1);
            set_color(Pixcels[i][j].green, values2);
            set_color(Pixcels[i][j].blue, values3);
        }
    }
}

