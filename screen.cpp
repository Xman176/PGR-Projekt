#include "screen.h"
#include "painterAlgorithm.h"
#include "zBufferPaint.h"

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
    zBuffer = new float[width*height];

    paintType = 0;
    paintBorders = false;
}

screen::~screen(){
    delete [] zBuffer;

    SDL_DestroyRenderer(_renderer);
}


void screen::ScreenSize(int w, int h){
    width = w;
    height = h;

    //realokace zBufferu
    delete [] zBuffer;
    zBuffer = new float[width*height];
}

void screen::ChangePaintType(int type){
    paintType = type;
}

void screen::ChangeBorder(){
    if(paintBorders)
        paintBorders = false;
    else
        paintBorders = true;
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

void screen::SortTrianglPoint(trianglePoints* oneTriangle){

        //zjisteni nejvyssiho bodu trojuhelniku
        if(oneTriangle->a->y <= oneTriangle->b->y){
            if(oneTriangle->a->y <= oneTriangle->c->y)
                top = oneTriangle->a;
            else
                top = oneTriangle->c;
        }
        else if(oneTriangle->b->y <= oneTriangle->c->y){
            top = oneTriangle->b;
        }
        else{
            top = oneTriangle->c;
        }


        //zjisteni leveho a praveho bodu trojuhelniku
        if(oneTriangle->c == top){
            if(oneTriangle->a->x < oneTriangle->b->x){
                left = oneTriangle->a;
                right = oneTriangle->b;
            }
            else{
                left = oneTriangle->b;
                right = oneTriangle->a;
            }
        }
        else if(oneTriangle->b == top){
            if(oneTriangle->a->x < oneTriangle->c->x){
                left = oneTriangle->a;
                right = oneTriangle->c;
            }
            else{
                left = oneTriangle->c;
                right = oneTriangle->a;
            }
        }
        else{
            if(oneTriangle->b->x < oneTriangle->c->x){
                left = oneTriangle->b;
                right = oneTriangle->c;
            }
            else{
                left = oneTriangle->c;
                right = oneTriangle->b;
            }
        }
}


void screen::PaintObject(){
    switch(paintType){
    case 0:
        painterAlg();
        break;

    case 1:
        paintWithZbuffer();
        break;
    }
}

void screen::Show(){
    //zobrazení vytvoøené plochy
    SDL_RenderPresent(_renderer);
}
