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
    void Move(int x, int y);
    void ChangeAngle(int moveX, int moveY);
    void Zoom(int x);

    void CleanRenderer();
    void PaintBackground(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void PaintObject();
    void PaintTriangle(point *top, point *left, point *right, point *normalVec);
    void Show();


private:
    SDL_Renderer* _renderer;

    rasterObject myObject;

    int width, height;

    int viewPosX, viewPosY;
    double zoom;

    float *zBuffer;
};




#endif
