#ifndef ZBUFFERPAINT_H
#define ZBUFFERPAINT_H

#include "screen.h"

void screen::PaintLineTriangleWithZ(){
    float zValue, zDirection;
    int startX, endX;
    int startDirection, endDirection;
    int lineInZBuffer = paintLine * width;
    float blueInc, redInc, greenInc;

    if(leftX < rightX){
        startX = floor(leftX);
        endX = floor(rightX);
        startDirection = ceil(fabs(leftDirection));
        endDirection = ceil(fabs(rightDirection));

        zValue = leftZ;
        zDirection = (leftZ - rightZ) / (startX - endX);

        if(myObject.colorObject && showSurface){
            Red = leftRed; Green = leftGreen; Blue = leftBlue;
            redInc = (leftRed - rightRed) / (startX - endX);
            greenInc = (leftGreen - rightGreen) / (startX - endX);
            blueInc = (leftBlue - rightBlue) / (startX - endX);
        }
    }
    else{
        startX = floor(rightX);
        endX = floor(leftX);
        startDirection = ceil(fabs(rightDirection));
        endDirection = ceil(fabs(leftDirection));

        zValue = rightZ;
        zDirection = (rightZ - leftZ) / (startX - endX);

        if(myObject.colorObject && showSurface){
            Red = rightRed; Green = rightGreen; Blue = rightBlue;
            redInc = (rightRed - leftRed) / (startX - endX);
            greenInc = (rightGreen - leftGreen) / (startX - endX);
            blueInc = (rightBlue - leftBlue) / (startX - endX);
        }
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

        if(myObject.colorObject && showSurface){
            Red += redInc;
            Green += greenInc;
            Blue += blueInc;
        }
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
