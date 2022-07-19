#include "def.h"

void setColor(PIGMENT& color, vector<double>& values){
    color = (PIGMENT)values[2];

    if((int)(color + (PIGMENT)values[1]) < 255){
        color += (PIGMENT)values[1];
    }
    else{
        color = 255;
        return;
    }

    if((int)(color + (PIGMENT)values[0]) < 255){
        color += (PIGMENT)values[0];
    }
    else{
        color = 255;
    }
}

void sepia(ImageThread& imageThread){
    vector<double> values1, values2, values3;
    for(int i = imageThread.firstRow; i < imageThread.lastRow; i++){
        for(int j = imageThread.firstColumn; j <= imageThread.lastColumn; j++){
            PIGMENT red = imageThread.imagePointingTo->pixcels[i][j].red;
            PIGMENT green = imageThread.imagePointingTo->pixcels[i][j].green;
            PIGMENT blue = imageThread.imagePointingTo->pixcels[i][j].blue; 
            values1 = {(red * 0.393), (green * 0.769), (blue * 0.189)};  
            sort(values1.begin(), values1.end());
            values2 = {(red * 0.349), (green * 0.686), (blue * 0.168)};
            sort(values2.begin(), values2.end());
            values3 = {(red * 0.272), (green * 0.534), (blue * 0.131)};
            sort(values3.begin(), values3.end());
            
            setColor(imageThread.imagePointingTo->pixcels[i][j].red, values1);
            setColor(imageThread.imagePointingTo->pixcels[i][j].green, values2);
            setColor(imageThread.imagePointingTo->pixcels[i][j].blue, values3);
        }
    }
}


