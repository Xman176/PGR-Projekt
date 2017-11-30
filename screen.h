#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <SDL2/SDL.h>

#include <cstring>
#include <math.h>

#include "rasterObject.h"
/*
typedef struct RGB{
    Uint8 R;
    Uint8 G;
    Uint8 B;
};*/

class screen{
public:
    screen(SDL_Renderer* _ren, int w, int h);
    ~screen();

    bool objectExist();

    void ScreenSize(int w, int h);
    void ChangePaintType(int type);
    void ChangeBorder();
    void changePaintSurface();

    void Move(int x, int y);
    void ChangeAngle(int moveX, int moveY);
    void ZAngleZoom(int moveZ, int zoom);
    void Zoom(int x);

    void CleanRenderer();
    void PaintBackground(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void PaintObject();
    void Show();


private:

    int paintType;

    bool paintBorders;
    bool showSurface;

    int viewPosX, viewPosY;
    int width, height;

    SDL_Renderer* _renderer;
    float *_zBuffer;
    //RGB *screenField;

    rasterObject myObject;

    //Funkce spolecne pro painterAlgorith a zBufferPaint (paint.cpp)
    void ComputeStartLineValue(bool zVal);
    void ComputeContinueLineValue(bool xVal);
    void PaintLineBorder(int *startX, int endBorder);
    void PaintLineBorder(int *startX, float *zValue, float zDirection, int endBorder);
    void ComputeNextLine();
    void PaintTriagles();
    void SortTrianglPoint(trianglePoints* oneTriangle);

    //Funkce v knihovne painterAlgorithm.cpp
    void PaintLineTriangle();
    void PaintTrianglePainter(point *normalVec);

    //Funkce v knihovne zBufferPaint.cpp
    void PaintLineTriangleWithZ();
    void PaintTriangleWithZbuffer(point *normalVec);

    //Body pro serazeni trojuhelnik k vykresleni
    point *top, *left, *right;

    // parametry pro vykreslovani trojuhelniku a posuvu
    int paintLine;
    double rightX, leftX;
    double rightDirection, leftDirection;
    int rightPointLine, leftPointLine;
    float zDirectionLeft, zDirectionRight, rightZ, leftZ;

};




#endif
