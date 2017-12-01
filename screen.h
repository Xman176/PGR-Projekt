#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <SDL2/SDL.h>

#include <cstring>
#include <math.h>

#include "rasterObject.h"

typedef struct RGBA{
    Uint8 B;
    Uint8 G;
    Uint8 R;
    Uint8 A;
}RGBA;

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
    SDL_Texture* _texture;
    float *_zBuffer;
    RGBA *screenField;

    rasterObject myObject;

    //Funkce spolecne pro painterAlgorith a zBufferPaint (paint.cpp)
    void ComputeStartLineValue(bool zVal);
    void ComputeContinueLineValue(bool xVal);
    void PaintLineBorder(int *startX, int endBorder);
    void PaintLineBorder(int *startX, float *zValue, float zDirection, int endBorder);
    void ComputeNextLine();
    void PaintTriagles();
    void SortTrianglPoint(trianglePoints* oneTriangle);
    void paintPoint(int xPosition, int yPosition);

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
    Uint8 Red, Green, Blue, Alpha;
    double rightX, leftX;
    double rightDirection, leftDirection;
    int rightPointLine, leftPointLine;
    float zDirectionLeft, zDirectionRight, rightZ, leftZ;

};




#endif
