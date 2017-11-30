#ifndef PAINTERALGORYTHM_H
#define PAINTERALGORYTHM_H

#include "screen.h"

void screen::PaintLineTriangle(){
    int startX, endX;
    int startDirection, endDirection;

    if(leftX < rightX){
        startX = floor(leftX);
        endX = floor(rightX);

        startDirection = ceil(fabs(leftDirection));
        endDirection = ceil(fabs(rightDirection));
    }
    else{
        startX = floor(rightX);
        endX = floor(leftX);

        startDirection = ceil(fabs(rightDirection));
        endDirection = ceil(fabs(leftDirection));
    }

    //std::cout << leftDirection << " -- " << rightDirection << " == " << leftX << " -- " << rightX << std::endl;

    if(leftDirection == INFINITY || rightDirection == INFINITY)
        startDirection = width;



    //Zrychleni pokud je konec <0 tak cely radek je mimo zobrazovaci plochu
    if(endX < 0 || startX > width){
        ComputeNextLine();
        return;
    }

    if(paintBorders){
        //std::cout << endDirection << " -- " << startDirection << " == " << startX << " -- " << startX + startDirection << std::endl;

        PaintLineBorder(&startX, startX + startDirection);
        endX -= endDirection;
    }

    //Pri nevykreslovani plochy se vykreslí pouze hranice
    if(showSurface){
        while(startX <= endX && startX < width){
            SDL_RenderDrawPoint(_renderer ,startX, paintLine);
            startX ++;
        }
    }

    if(paintBorders && startX < width)
        PaintLineBorder(&endX, endDirection + endX);


    //inkrementace hodnot pro dalsi radek
    ComputeNextLine();

}


void screen::PaintTrianglePainter(point *normalVec){

    //velikost tohoto vektoru staci spocitat pri inicializaci obrazce
    double norVecLenght = sqrt(pow(normalVec->x,2)+pow(normalVec->y,2)+pow(normalVec->z,2));
    int grayColor = fabs(normalVec->z/norVecLenght) * 255;

    //inicializace barvy vekresleneho trojuhleniku
    SDL_SetRenderDrawColor(_renderer, grayColor, grayColor, grayColor, 255);

    ComputeStartLineValue(false);

    //Vykresleni prvni poloviny trojuhelniku
    while (paintLine < leftPointLine && paintLine < rightPointLine){
        if(paintLine > height)
            return;
        else if(paintLine <= 0)
            ComputeNextLine();
        else
            PaintLineTriangle();
    }

    //Novy prepocet smernic
    ComputeContinueLineValue(false);

    //Vykresli zbytek trojuhelniku
    while (paintLine < leftPointLine || paintLine < rightPointLine){
        if(paintLine > height)
            return;
        else if(paintLine <= 0)
            ComputeNextLine();
        else
            PaintLineTriangle();
    }


}

#endif // PAINTERALGORYTHM_H
