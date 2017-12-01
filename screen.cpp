#include "screen.h"

#include <math.h>

screen::screen(SDL_Renderer* _ren, int w, int h){
    _renderer = _ren;

    _texture = SDL_CreateTexture(_renderer,
                SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, w, h);

    //nastaveni sirtky okna
    width = w;
    height = h;

    //pohled na stred
    viewPosX = width / 2;
    viewPosY = height / 2;

    //alokace zBufferu
    _zBuffer = new float[width*height];
    screenField = new RGBA[width*height];

    paintType = 1;
    paintBorders = false;
    showSurface = true;

    myObject.Zoom((width/4.0)/myObject.initZoom);
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

    delete [] screenField;
    screenField = new RGBA[width*height];

    SDL_DestroyTexture(_texture);
    _texture = SDL_CreateTexture(_renderer,
                SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, w, h);
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

void screen::SetBorder(bool val){
    paintBorders = val;
}

void screen::SetSurface(bool val){
    showSurface = val;
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

void screen::PaintObject(){
    PaintTriagles();

    SDL_UpdateTexture(_texture, NULL, screenField, width*sizeof(RGBA));

    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, _texture, NULL, NULL);
    SDL_RenderPresent(_renderer);

}

void screen::changePaintSurface(){
    showSurface = (showSurface == 0) ? 1 : 0;
}

void screen::CenterObject(){
    viewPosX = width / 2;
    viewPosY = height / 2;
}
