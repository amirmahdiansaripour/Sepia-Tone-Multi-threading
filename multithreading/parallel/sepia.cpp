#include "def.h"

void set_color(PIGMENT& a, vector<double>& values){
    a = (PIGMENT)values[2];

    if((int)(a + (PIGMENT)values[1]) < 255)
        a += (PIGMENT)values[1];
    else{
        a = 255;
        return;
    }

    if((int)(a + (PIGMENT)values[0]) < 255)
        a += (PIGMENT)values[0];
    else
        a = 255;
}

void sepia(int firstRow, int lastRow, int firstColumn , int lastColumn , Image& image){
    vector<double> values1, values2, values3;
    for(int i = firstRow; i <= lastRow; i++){
        for(int j = firstColumn; j <= lastColumn; j++){
            values1 = {(image.pixcels[i][j].red * 0.393), (image.pixcels[i][j].green * 0.769), (image.pixcels[i][j].blue * 0.189)};  
            sort(values1.begin(), values1.end());
            values2 = {(image.pixcels[i][j].red * 0.349), (image.pixcels[i][j].green * 0.686), (image.pixcels[i][j].blue * 0.168)};
            sort(values2.begin(), values2.end());
            values3 = {(image.pixcels[i][j].red * 0.272), (image.pixcels[i][j].green * 0.534), (image.pixcels[i][j].blue * 0.131)};
            sort(values3.begin(), values3.end());
            

            set_color(image.pixcels[i][j].red, values1);
            set_color(image.pixcels[i][j].green, values2);
            set_color(image.pixcels[i][j].blue, values3);
        }
    }
}


