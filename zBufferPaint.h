#ifndef ZBUFFERPAINT_H
#define ZBUFFERPAINT_H

#include "screen.h"

struct dataZTriangle{
    int paintLine;
    double rightX, leftX;
    double rightDirection, leftDirection;
    float zDirectionLeft, zDirectionRight, rightZ, leftZ;
    int rightPointLine, leftPointLine;
    int screenWidth, screenHeight;
};

void ComputeNextLine(dataZTriangle *_data){
        //Posun podle vypocitaneho smeru pro dalsi radek pixelu
        _data->leftX += _data->leftDirection;
        _data->rightX += _data->rightDirection;

        //Posun na urovni z pro z-Buffer
        _data->leftZ += _data->zDirectionLeft;
        _data->rightZ += _data->zDirectionRight;

        //Posun na dalsi radek
        _data->paintLine ++;
}

void PaintPointBorder(SDL_Renderer *_renderer, int xPointPos, int yPointPos){
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(_renderer, &r, &g, &b, &a);

    //nastaveni barvy pro vykreslovani hranice a vykresli bod hranice
    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(_renderer ,xPointPos, yPointPos);

    //Vrat zpet barvu obsahu trojuhelniku
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);


}

void paintLineTriangleWithZ(SDL_Renderer *_renderer, dataZTriangle *_data, float *_zBuffer, bool paintBorder){
        float zValue, zDirection;
        int startX, endX;
        int lineInZBuffer = _data->paintLine * _data->screenWidth;

        if(_data->leftX < _data->rightX){
            startX = floor(_data->leftX);
            endX = floor(_data->rightX);

            zValue = _data->leftZ;
            zDirection = (_data->leftZ - _data->rightZ) / (startX - endX);
        }
        else{
            startX = floor(_data->rightX);
            endX = floor(_data->leftX);

            zValue = _data->rightZ;
            zDirection = (_data->rightZ - _data->leftZ) / (startX - endX);

        }

        //Zrychleni pokud je konec <0 tak cely radek je mimo zobrazovaci plochu
        if(endX < 0 || startX > _data->screenWidth){
            ComputeNextLine(_data);
            return;
        }

        if(paintBorder){
            if(zValue >= _zBuffer[lineInZBuffer + startX]){
                PaintPointBorder(_renderer, startX, _data->paintLine);
                _zBuffer[lineInZBuffer + startX] = zValue;
            }

            zValue += zDirection;
            startX ++;
            endX --;
        }

        //Vykresleni jednoho radku
        while(startX <= endX){
            if(startX > _data->screenWidth)
                break;

            if(zValue >= _zBuffer[lineInZBuffer + startX]){
                SDL_RenderDrawPoint(_renderer ,startX, _data->paintLine);
                _zBuffer[lineInZBuffer + startX] = zValue;
            }

            zValue += zDirection;
            startX ++;
        }

        if(paintBorder && startX < endX++){
            if(zValue >= _zBuffer[lineInZBuffer + endX]){
                PaintPointBorder(_renderer, endX, _data->paintLine);
                _zBuffer[lineInZBuffer + endX] = zValue;
            }
        }

        ComputeNextLine(_data);

}

void screen::paintWithZbuffer(){
    int objectCount = myObject.GetTrianCount();
    trianglePoints *oneTriangle;

    //vytvoreni promenne pro vektor normaly trojuhelniku
    point *normalVec;
    normalVec = new point;

    //vycisteni z-Bufferu (bez pouziti cyklu for)
    std::fill_n(zBuffer, width*height, -INFINITY);


    //cyklus pro vykresleni kazdeho trojuhelniku
    for(int i = 0; i < objectCount; i ++){
        //ziskani trojuhelniku ze seznamu v objektu
        oneTriangle = myObject.GetTriangl(i);

        SortTrianglPoint(oneTriangle);

        //ziskani normaly trojuhelniku
        myObject.GetNormal(oneTriangle, normalVec);

        //Vykreslit trojuhelnik
        PaintTriangleWithZbuffer(normalVec);
    }
}

void screen::PaintTriangleWithZbuffer(point *normalVec){
    //velikost tohoto vektoru staci spocitat pri inicializaci obrazce
    double norVecLenght = sqrt(pow(normalVec->x,2)+pow(normalVec->y,2)+pow(normalVec->z,2));
    int grayColor = fabs(normalVec->z/norVecLenght) * 191 + 64;

    //inicializace barvy vekresleneho trojuhleniku
    SDL_SetRenderDrawColor(_renderer, grayColor, grayColor, grayColor, 255);


     dataZTriangle *_data = new dataZTriangle;

     _data->screenHeight = height;
     _data->screenWidth = width;

    //Implementace vykresleni trojuhelniku pomoci DDA
    _data->leftX = top->x + viewPosX;
    _data->rightX = _data->leftX;

    //promenna pro oznaceni vykreslovaneho radku
    _data->paintLine = top->y + viewPosY;

    //Vypocet smernice pro 3. a 4. kvadrant
    _data->leftDirection = (double)(left->x - top->x) / (double)(left->y - top->y);
    _data->rightDirection = (double)(right->x - top->x) / (double)(right->y - top->y);

    //vypocet radku k leveho a praveho bodu trojuhelniku
    _data->leftPointLine = left->y + viewPosY;
    _data->rightPointLine = right->y + viewPosY;

    //vypocet hodnot pro Z souradnice
    _data->rightZ = top->z;
    _data->leftZ = _data->rightZ;
    _data->zDirectionLeft = (left->z - top->z) / (left->y - top->y);
    _data->zDirectionRight = (right->z - top->z) / (right->y - top->y);


    //Vykresleni prvni poloviny trojuhelniku
    while (_data->paintLine < _data->leftPointLine && _data->paintLine < _data->rightPointLine){
        //podminky pro nevykreslovani radku trojuhelniku mimo obrazovku
        if(_data->paintLine > height)
            return;
        else if(_data->paintLine <= 0)
            ComputeNextLine(_data);

        else
            paintLineTriangleWithZ(_renderer, _data, zBuffer, paintBorders);
    }


    // urceni ktereho bodu bylo dosazeno drive, upraveni smernice pro dalsi vykresleni
    if(_data->paintLine >= _data->leftPointLine){
        _data->leftDirection = (right->x - left->x) / (right->y - left->y);
        _data->leftX = left->x + viewPosX;
        _data->zDirectionLeft = (right->z - left->z) / (right->y - left->y);
        _data->leftZ = left->z;
    }
    else if(_data->paintLine >= _data->rightPointLine){
        _data->rightDirection = (left->x - right->x) / (left->y - right->y);
        _data->rightX = right->x + viewPosX;
        _data->zDirectionRight = (left->z - right->z) / (left->y - right->y);
        _data->rightZ = right->z;

    }

    //Vykresli zbytek trojuhelniku
    while (_data->paintLine < _data->leftPointLine || _data->paintLine < _data->rightPointLine){
        //podminky pro nevykreslovani radku trojuhelniku mimo obrazovku
        if(_data->paintLine > height)
            return;
        else if(_data->paintLine <= 0)
            ComputeNextLine(_data);

        else
            paintLineTriangleWithZ(_renderer, _data, zBuffer, paintBorders);
    }
}


#endif // ZBUFFERPAINT_H
