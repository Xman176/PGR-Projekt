#ifndef ZBUFFERPAINT_H
#define ZBUFFERPAINT_H

#include "screen.h"

void screen::PaintLineTriangleWithZ(){
    float zValue, zDirection;
    int startX, endX;
    int startDirection, endDirection;
    int lineInZBuffer = paintLine * width;

    if(leftX < rightX){
        startX = floor(leftX);
        endX = floor(rightX);
        startDirection = ceil(fabs(leftDirection));
        endDirection = ceil(fabs(rightDirection));

        zValue = leftZ;
        zDirection = (leftZ - rightZ) / (startX - endX);
    }
    else{
        startX = floor(rightX);
        endX = floor(leftX);
        startDirection = ceil(fabs(rightDirection));
        endDirection = ceil(fabs(leftDirection));

        zValue = rightZ;
        zDirection = (rightZ - leftZ) / (startX - endX);

    }

    if(leftDirection == INFINITY || rightDirection == INFINITY)
        startDirection = width;


    //Zrychleni pokud je konec <0 tak cely radek je mimo zobrazovaci plochu
    if(endX < 0 || startX > width){
        ComputeNextLine();
        return;
    }

    if(paintBorders){
        PaintLineBorder(&startX, &zValue, zDirection, abs(startDirection) + startX);
        endX -= abs(endDirection);
    }

    //Vykresleni jednoho radku
    while(startX < endX){
        if(startX > width)
            break;

        if(zValue > _zBuffer[lineInZBuffer + startX]){
            paintPoint(startX, paintLine);
            //SDL_RenderDrawPoint(_renderer ,startX, paintLine);
            _zBuffer[lineInZBuffer + startX] = zValue;
        }

        zValue += zDirection;
        startX ++;
    }

    if(paintBorders && startX < width){
            PaintLineBorder(&endX, &zValue, zDirection, abs(endDirection) + endX);
    }

    ComputeNextLine();

}

void screen::PaintTriangleWithZbuffer(point *normalVec){
    int grayColor;
    //velikost tohoto vektoru staci spocitat pri inicializaci obrazce, pokud se nema zobrazit tak bude cerny
    if(!showSurface){
        grayColor = 0;
    }
    else{
        double norVecLenght = sqrt(pow(normalVec->x,2)+pow(normalVec->y,2)+pow(normalVec->z,2));
        grayColor = fabs(normalVec->z/norVecLenght) * 191 + 64;
    }

    //inicializace barvy vekresleneho trojuhleniku
    Red = grayColor; Green = grayColor; Blue = grayColor; Alpha = 255;
    //SDL_SetRenderDrawColor(_renderer, grayColor, grayColor, grayColor, 255);

    //Vypocet pocatecni hodnot posuvu pro jednotlivé radky
    ComputeStartLineValue(true);

    //Vykresleni prvni poloviny trojuhelniku
    while (paintLine < leftPointLine && paintLine < rightPointLine){
        //podminky pro nevykreslovani radku trojuhelniku mimo obrazovku
        if(paintLine > height)
            return;
        else if(paintLine <= 0)
            ComputeNextLine();
        else
            PaintLineTriangleWithZ();
    }


    // urceni ktereho bodu bylo dosazeno drive, upraveni smernice pro dalsi vykresleni
    ComputeContinueLineValue(true);

    //Vykresli zbytek trojuhelniku
    while (paintLine < leftPointLine || paintLine < rightPointLine){
        //podminky pro nevykreslovani radku trojuhelniku mimo obrazovku
        if(paintLine > height)
            return;
        else if(paintLine <= 0)
            ComputeNextLine();
        else
            PaintLineTriangleWithZ();
    }

}


#endif // ZBUFFERPAINT_H
