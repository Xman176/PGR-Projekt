
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

    if(myObject.colorObject && showSurface){
        leftRed = rightRed = top->r;
        leftGreen = rightGreen = top->g;
        leftBlue = rightBlue = top->b;

        lDirRed = (top->r - left->r) / (left->y - top->y);
        lDirGreen = (top->g - left->g) / (left->y - top->y);
        lDirBlue = (top->b - left->b) / (left->y - top->y);

        rDirRed = (top->r - right->r) / (right->y - top->y);
        rDirGreen = (top->g - right->g) / (right->y - top->y);
        rDirBlue = (top->b - right->b) / (right->y - top->y);
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

        if(myObject.colorObject){
            leftRed = left->r;
            leftGreen = left->g;
            leftBlue = left->b;

            rDirRed = (left->r - right->r) / (right->y - left->y);
            rDirGreen = (left->g - right->g) / (right->y - left->y);
            rDirBlue = (left->b - right->b) / (right->y - left->y);
        }
    }
    else if(paintLine >= rightPointLine){
        rightDirection = (left->x - right->x) / (left->y - right->y);
        rightX = right->x + viewPosX;

        if(zVal){
            zDirectionRight = (left->z - right->z) / (left->y - right->y);
            rightZ = right->z;
        }

        if(myObject.colorObject && showSurface){
            rightRed = right->r;
            rightGreen = right->g;
            rightBlue = right->b;

            lDirRed = (right->r - left->r) / (left->y - right->y);
            lDirGreen = (right->g - left->g) / (left->y - right->y);
            lDirBlue = (right->b - left->b) / (left->y - right->y);;
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


    Uint8 r, g, b;
    r = Red; g = Green; b = Blue;
    //SDL_GetRenderDrawColor(_renderer, &r, &g, &b, &a);
    Red = 255; Green = 0; Blue = 0;
    //nastaveni barvy pro vykreslovani hranice a vykresli bod hranice
    //SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);

    do{
        paintPoint(*startX, paintLine);
        //SDL_RenderDrawPoint(_renderer ,*startX, paintLine);
        (*startX)++;
    }while(*startX < endBorder);

    //Vrat zpet barvu obsahu trojuhelniku
    Red = r; Green = g; Blue = b;
    //SDL_SetRenderDrawColor(_renderer, r, g, b, a);
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
    Uint8 r, g, b;
    r = Red; g = Green; b = Blue;
    //SDL_GetRenderDrawColor(_renderer, &r, &g, &b, &a);
    Red = 255; Green = 0; Blue = 0;
    //nastaveni barvy pro vykreslovani hranice a vykresli bod hranice
    //SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);

    do{
        if(*zValue >= _zBuffer[lineInZBuffer + *startX]){
                 paintPoint(*startX, paintLine);
                //SDL_RenderDrawPoint(_renderer ,*startX, paintLine);
                _zBuffer[lineInZBuffer + *startX] = *zValue;
        }

        (*zValue) += zDirection;
        (*startX)++;
    }while(*startX < endBorder);
    //Vrat zpet barvu obsahu trojuhelniku
    Red = r; Green = g; Blue = b;
    //SDL_SetRenderDrawColor(_renderer, r, g, b, a);
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

        //prepocet barvy
        if(myObject.colorObject && showSurface){
            leftRed += lDirRed;
            leftGreen += lDirGreen;
            leftBlue += lDirBlue;

            rightRed += rDirRed;
            rightGreen += rDirGreen;
            rightBlue += rDirBlue;
        }
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

    memset(screenField, 0, sizeof(RGBA)*width*height);

    //cyklus pro vykresleni kazdeho trojuhelniku
    for(int i = 0; i < objectCount; i ++){
        //ziskani trojuhelniku ze seznamu v objektu
        oneTriangle = myObject.GetTriangl(i);

        SortTrianglPoint(oneTriangle);

        //ziskani normaly trojuhelniku
        myObject.GetNormal(oneTriangle, normalVec);

        //Vykreslit trojuhelnik
        switch (paintType){
        case 1:
            PaintTriangleWithZbuffer(normalVec);
            break;
        case 2:
            PaintTrianglePainter(normalVec);
            break;

        }

    }
}

void screen::paintPoint(int xPosition, int yPosition){
    if((xPosition < width && xPosition >= 0) && (yPosition < height-1 && yPosition >= 0)){
        int position = xPosition + width * yPosition;

        screenField[position].R = Red;
        screenField[position].G = Green;
        screenField[position].B = Blue;
        screenField[position].A = Alpha;

    }
}


