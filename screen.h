#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <SDL2/SDL.h>

#include <cstring>
#include <math.h>

#include "rasterObject.h"


class screen{
public:
    screen(SDL_Renderer* _ren, int w, int h);
    ~screen();

    void ScreenSize(int w, int h);
    void ChangePaintType(int type);
    void ChangeBorder();
    void changePaintSurface();

    void Move(int x, int y);
    void ChangeAngle(int moveX, int moveY);
    void Zoom(int x);

    void CleanRenderer();
    void PaintBackground(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void PaintObject();
    void Show();


private:
    //Funkce v knihovne painterAlgorithm.h
    void painterAlg();
    void PaintTrianglePainter(point *normalVec);

    //Funkce v knihovne zBufferPaint.h
    void paintWithZbuffer();
    void PaintTriangleWithZbuffer(point *normalVec);

    //Funkce v screen.cpp
    void SortTrianglPoint(trianglePoints* oneTriangle);


    SDL_Renderer* _renderer;

    rasterObject myObject;

    int width, height;

    int paintType;
    bool paintBorders;
    bool showSurface;

    point *top, *left, *right;

    int viewPosX, viewPosY;

    float *zBuffer;
};




#endif
