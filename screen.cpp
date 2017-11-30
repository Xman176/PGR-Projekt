#include "screen.h"

#include <math.h>

screen::screen(SDL_Renderer* _ren, int w, int h){
    _renderer = _ren;

    //nastaveni sirtky okna
    width = w;
    height = h;

    //pohled na stred
    viewPosX = width / 2;
    viewPosY = height / 2;

    //alokace zBufferu
    _zBuffer = new float[width*height];
    //screenField = new RGB[width*height];

    paintType = 0;
    paintBorders = false;
    showSurface = true;
}

screen::~screen(){
    delete [] _zBuffer;

    SDL_DestroyRenderer(_renderer);
}


void screen::ScreenSize(int w, int h){
    width = w;
    height = h;

    //realokace zBufferu
    delete [] _zBuffer;
    _zBuffer = new float[width*height];

//    delete [] screenField;
//    screenField = new RGB[width*height];
}

bool screen::objectExist(){
    return myObject.objectOK;
}

void screen::ChangePaintType(int type){
    paintType = type;
}

void screen::ChangeBorder(){
    paintBorders = (paintBorders == 0) ? 1 : 0;
}

void screen::Move(int x, int y){
    viewPosX += x*10;
    viewPosY += y*10;
}

void screen::Zoom(int x){
    myObject.Zoom(1.0 + x/10.0);

}

void screen::ChangeAngle(int moveX, int moveY){
    if(moveX != 0)
        myObject.RotateX(moveX / 100.0);

    if(moveY != 0)
        myObject.RotateY(moveY / 100.0);

}

void screen::ZAngleZoom(int moveZ, int zoom){
    if(zoom != 0)
        //myObject.Zoom(1-zoom/100.0);

    if(moveZ != 0)
        myObject.RotateZ(moveZ / 100.0);
}

void screen::CleanRenderer(){
    SDL_RenderClear(_renderer);
}


void screen::PaintBackground(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
    //Vyplneni pozadi cernou barvou
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);

    SDL_Rect rect;
    rect.x = rect.y = 0;
    rect.h = height;
    rect.w = width;

    SDL_RenderFillRect(_renderer, &rect);

}

void screen::PaintObject(){
    PaintTriagles();
}

void screen::changePaintSurface(){
    showSurface = (showSurface == 0) ? 1 : 0;
}

void screen::Show(){
    //zobrazení vytvoøené plochy
    SDL_RenderPresent(_renderer);
}
