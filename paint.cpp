
#include "screen.h"
void screen::ComputeStartLineValue(bool zVal){
    //Implementace vykresleni trojuhelniku pomoci DDA
    if(left->y == top->y){
        leftX = left->x + viewPosX;
        rightX = top->x + viewPosX;
    }
    else if(right->y == top->y){
        leftX = top->x + viewPosX;
        rightX = right->x + viewPosX;
    }
    else{
        leftX = top->x + viewPosX;
        rightX = leftX;
    }

    //promenna pro oznaceni vykreslovaneho radku
    paintLine = top->y + viewPosY;

    //Vypocet smernice pro 3. a 4. kvadrant
    leftDirection = (double)(left->x - top->x) / (double)(left->y - top->y);
    rightDirection = (double)(right->x - top->x) / (double)(right->y - top->y);

    //vypocet radku k leveho a praveho bodu trojuhelniku
    leftPointLine = left->y + viewPosY;
    rightPointLine = right->y + viewPosY;

    if(zVal){
        //vypocet hodnot pro Z souradnice
        rightZ = top->z;
        leftZ = rightZ;
        zDirectionLeft = (left->z - top->z) / (left->y - top->y);
        zDirectionRight = (right->z - top->z) / (right->y - top->y);
    }
}

void screen::ComputeContinueLineValue(bool zVal){
    // urceni ktereho bodu bylo dosazeno drive, upraveni smernice pro dalsi vykresleni
    if(paintLine >= leftPointLine){
        leftDirection = (right->x - left->x) / (right->y - left->y);
        leftX = left->x + viewPosX;

        if(zVal){
            zDirectionLeft = (right->z - left->z) / (right->y - left->y);
            leftZ = left->z;
        }
    }
    else if(paintLine >= rightPointLine){
        rightDirection = (left->x - right->x) / (left->y - right->y);
        rightX = right->x + viewPosX;

        if(zVal){
            zDirectionRight = (left->z - right->z) / (left->y - right->y);
            rightZ = right->z;
        }
    }
}

void screen::PaintLineBorder(int *startX, int endBorder){
    if(leftX < rightX){
        if(*startX < leftX)
           *startX = leftX;

        if(endBorder > rightX)
            endBorder = rightX;
    }
    else{
       if(*startX < rightX)
           *startX = rightX;

        if(endBorder > leftX)
            endBorder = leftX;
    }


    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(_renderer, &r, &g, &b, &a);

    //nastaveni barvy pro vykreslovani hranice a vykresli bod hranice
    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);

    do{
        SDL_RenderDrawPoint(_renderer ,*startX, paintLine);
        (*startX)++;
    }while(*startX < endBorder);

    //Vrat zpet barvu obsahu trojuhelniku
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
}

void screen::PaintLineBorder(int *startX, float *zValue, float zDirection, int endBorder){
    if(leftX < rightX){
        if(*startX < leftX)
           *startX = leftX;

        if(endBorder > rightX)
            endBorder = rightX;
    }
    else{
       if(*startX < rightX)
           *startX = rightX;

        if(endBorder > leftX)
            endBorder = leftX;
    }

    int lineInZBuffer = paintLine * width;
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(_renderer, &r, &g, &b, &a);

    //nastaveni barvy pro vykreslovani hranice a vykresli bod hranice
    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);

    do{
        if(*zValue >= _zBuffer[lineInZBuffer + *startX]){
                SDL_RenderDrawPoint(_renderer ,*startX, paintLine);
                _zBuffer[lineInZBuffer + *startX] = *zValue;
        }

        (*zValue) += zDirection;
        (*startX)++;
    }while(*startX < endBorder);
    //Vrat zpet barvu obsahu trojuhelniku
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
}


void screen::ComputeNextLine(){
        //Posun podle vypocitaneho smeru pro dalsi radek pixelu
        leftX += leftDirection;
        rightX += rightDirection;

        //Posun na urovni z pro z-Buffer
        leftZ += zDirectionLeft;
        rightZ += zDirectionRight;

        //Posun na dalsi radek
        paintLine ++;
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

void screen::PaintTriagles(){
    int objectCount = myObject.GetTrianCount();
    trianglePoints *oneTriangle;

    //vytvoreni promenne pro vektor normaly trojuhelniku
    point *normalVec;
    normalVec = new point;

    if(paintType == 1)
        std::fill_n(_zBuffer, width*height, -INFINITY);

    //cyklus pro vykresleni kazdeho trojuhelniku
    for(int i = 0; i < objectCount; i ++){
        //ziskani trojuhelniku ze seznamu v objektu
        oneTriangle = myObject.GetTriangl(i);

        SortTrianglPoint(oneTriangle);

        //ziskani normaly trojuhelniku
        myObject.GetNormal(oneTriangle, normalVec);

        //Vykreslit trojuhelnik
        switch (paintType){
        case 0:
            PaintTrianglePainter(normalVec);
            break;
        case 1:
            PaintTriangleWithZbuffer(normalVec);
            break;
        }

    }
}

/*void screen::paintPoint(Uint8 R, Uint8 G, Uint8 B, int xPosition, int yPosition){



}*/


