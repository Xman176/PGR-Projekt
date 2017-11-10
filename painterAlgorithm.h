#ifndef PAINTERALGORYTHM_H
#define PAINTERALGORYTHM_H

#include "screen.h"

struct paintTriangleData{
    int paintLine;
    double rightX, leftX;
    double rightDirection, leftDirection;
    int rightPointLine, leftPointLine;
};

void paintLineTriangle(SDL_Renderer *_renderer, paintTriangleData *_data){
        if(_data->leftX < _data->rightX){
            for(int i = _data->leftX; i <= _data->rightX; i++){
                SDL_RenderDrawPoint(_renderer ,i, _data->paintLine);
            }
        }
        else{
            for(int i = _data->rightX; i <= _data->leftX; i++)
                SDL_RenderDrawPoint(_renderer ,i, _data->paintLine);

        }

        //Posun podle vypocitaneho smeru
        _data->leftX += _data->leftDirection;
        _data->rightX += _data->rightDirection;
        //Posun na dalsi radek
        _data->paintLine ++;
}


void screen::painterAlg(){
    int objectCount = myObject.GetTrianCount();
    trianglePoints *oneTriangle;

    //vytvoreni promenne pro vektor normaly trojuhelniku
    point *normalVec;
    normalVec = new point;

    //cyklus pro vykresleni kazdeho trojuhelniku
    for(int i = 0; i < objectCount; i ++){
        //ziskani trojuhelniku ze seznamu v objektu
        oneTriangle = myObject.GetTriangl(i);

        SortTrianglPoint(oneTriangle);

        //ziskani normaly trojuhelniku
        myObject.GetNormal(oneTriangle, normalVec);

        //Vykreslit trojuhelnik
        PaintTrianglePainter(normalVec);
    }
}


void screen::PaintTrianglePainter(point *normalVec){

    //velikost tohoto vektoru staci spocitat pri inicializaci obrazce
    double norVecLenght = sqrt(pow(normalVec->x,2)+pow(normalVec->y,2)+pow(normalVec->z,2));
    int grayColor = fabs(normalVec->z/norVecLenght) * 255;

    //inicializace barvy vekresleneho trojuhleniku
    SDL_SetRenderDrawColor(_renderer, grayColor, grayColor, grayColor, 255);

    paintTriangleData *_data = new paintTriangleData;

    //Implementace vykresleni trojuhelniku pomoci DDA
    _data->leftX = top->x + viewPosX;
    _data->rightX = _data->leftX;

    //promenna pro oznaceni vykreslovaneho radku
    _data->paintLine = top->y + viewPosY;

    //Vypocet smernice pro 3. a 4. kvadrant
    _data->leftDirection = (double)(left->x - top->x) / (double)(left->y - top->y);
    _data->rightDirection = (double)(right->x - top->x) / (double)(right->y - top->y);

    //vypocet radku k levemu a pravemu bodu trojuhelniku
    _data->leftPointLine = left->y + viewPosY;
    _data->rightPointLine = right->y + viewPosY;

    //Vykresleni prvni poloviny trojuhelniku
    while (_data->paintLine < _data->leftPointLine && _data->paintLine < _data->rightPointLine)
        paintLineTriangle(_renderer, _data);


    // urceni ktereho bodu bylo dosazeno drive, upraveni smernice pro dalsi vykresleni
    if(_data->paintLine >= _data->leftPointLine){
        _data->leftDirection = (right->x - left->x) / (right->y - left->y);
        _data->leftX = left->x + viewPosX;
    }
    else if(_data->paintLine >= _data->rightPointLine){
        _data->rightDirection = (left->x - right->x) / (left->y - right->y);
        _data->rightX = right->x + viewPosX;
    }

    //Vykresli zbytek trojuhelniku
    while (_data->paintLine < _data->leftPointLine || _data->paintLine < _data->rightPointLine)
        paintLineTriangle(_renderer, _data);

    //Vykresli hranice jednotlivych trojuhelniku
    if(paintBorders){
        SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(_renderer, top->x+viewPosX, top->y+viewPosY, left->x+viewPosX, left->y+viewPosY);
        SDL_RenderDrawLine(_renderer, top->x+viewPosX, top->y+viewPosY, right->x+viewPosX, right->y+viewPosY);
        SDL_RenderDrawLine(_renderer, right->x+viewPosX, right->y+viewPosY, left->x+viewPosX, left->y+viewPosY);
    }

    delete _data;
}

#endif // PAINTERALGORYTHM_H
