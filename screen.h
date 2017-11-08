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


    void SortTrianglPoint(trianglePoints* oneTriangle);
    void PaintTriangle(point *normalVec);


    SDL_Renderer* _renderer;

    rasterObject myObject;

    int width, height;

    int paintType;

    point *top, *left, *right;

    int viewPosX, viewPosY;

    float *zBuffer;
};




#endif
