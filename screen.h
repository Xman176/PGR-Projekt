#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <SDL2/SDL.h>

#include <cstring>
#include <math.h>

#include "rasterObject.h"

typedef struct Pixel{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
}Pixel;


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
    void Show();


private:
    void SortTrianglPoint(trianglePoints* oneTriangle);
    void PaintTriangle(point *normalVec);


    SDL_Renderer* _renderer;

    rasterObject myObject;

    int width, height;

    point *top, *left, *right;

    int viewPosX, viewPosY;
    double zoom;

    float *zBuffer;
};




#endif
