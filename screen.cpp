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

    //nastaveni zakladniho zvetseni
    zoom = 1.0;

    //alokace zBufferu
    zBuffer = new float[width*height];
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

void screen::Move(int x, int y){
    viewPosX += x*10;
    viewPosY += y*10;
}

void screen::Zoom(int x){
    zoom += x*10;
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

    int objectCount = myObject.GetTrianCount();
    trianglePoints *oneTriangle;

    //Vyčisteni z-Bufferu
    for(int i = 0; i < width * height; i++)
        zBuffer[i] = -INFINITY;

    //vytvoreni promenne pro vektor normaly trojuhelniku
    point *normalVec;
    normalVec = new point;

    //cyklus pro vykresleni kazdeho trojuhelniku
    for(int i = 0; i < objectCount; i ++){
        //ziskani trojuhelniku ze seznamu v objektu
        oneTriangle = myObject.GetTriangl(i);

        SortTrianglPoint(oneTriangle);

        //ziskani normaly trojuhelniku
        myObject.GetNormal(oneTriangle, normalVec);

        //Vykreslit trojuhelnik
        PaintTriangle(normalVec);
    }
}

void screen::PaintTriangle(point *normalVec){

    //velikost tohoto vektoru staci spocitat pri inicializaci obrazce
    double norVecLenght = sqrt(pow(normalVec->x,2)+pow(normalVec->y,2)+pow(normalVec->z,2));

    int grayColor = fabs(normalVec->z/norVecLenght) * 255;

    //inicializace barvy vekresleneho trojuhleniku
    SDL_SetRenderDrawColor(_renderer, grayColor, grayColor, grayColor, 255);

    //Implementace vykresleni trojuhelniku pomoci DDA
    double leftX = top->x * zoom + viewPosX;
    double rightX = leftX;

    //promenna pro oznaceni vykreslovaneho radku
    int paintLine = top->y * zoom + viewPosY;

    //Vypocet smernice pro 3. a 4. kvadrant
    double leftDirection = (double)(left->x - top->x) / (double)(left->y - top->y);
    double rightDirection = (double)(right->x - top->x) / (double)(right->y - top->y);

    //vypocet radku leveho a praveho radku
    int leftPointLine = left->y * zoom + viewPosY;
    int rightPointLine = right->y * zoom + viewPosY;

    //promenne pro vypocet hladiny z
    double leftZDirection = ((left->z - top->z)*zoom)/(leftPointLine - paintLine);
    double rightZDirection = ((right->z - top->z)*zoom)/(rightPointLine - paintLine);

    double leftZ = top->z * zoom;
    double rightZ = leftZ;

    //std::cout << "Paint line: " << paintLine << "  --  " << leftPointLine << "  --  " << rightPointLine << "  x  " << leftDirection << "  --  " << rightDirection << std::endl;
    //std::cout << "Point top: " << top->x * zoom + viewPosX << "  --  " << top->y * zoom + viewPosY << "  Left:  " << left->x * zoom + viewPosX << "  --  " << left->y * zoom + viewPosY << "  RIGHT:  " << right->x * zoom + viewPosX << "  --  " << right->y * zoom + viewPosY << std::endl;
    //std::cout << "Left X: " << leftX << "  <=  " << rightX << std::endl;
    //std::cout << "Normal: " << normalVec->x << "  --  " << normalVec->y << "  --  " << normalVec->z << "  ==  " << norVecLenght << std::endl;
    //std::cout << "Top Z: " << top->z*zoom << "  --  " << left->z*zoom << "  --  " << right->z*zoom << std::endl;
    //std::cout << "Vector: " << leftZDirection << "  --  " << rightZDirection << "  --  " << (leftPointLine - paintLine) << "  --  " <<(rightPointLine - paintLine)<< std::endl;
    //std::cout << "totalMove " << (leftPointLine - paintLine)*leftZDirection << "  --  " << (rightPointLine - paintLine)*rightZDirection << std::endl;

    while (paintLine < leftPointLine && paintLine < rightPointLine){
        int lineInZBuffer = paintLine * width;
        double z, zDirection;

        if(leftX < rightX){
            zDirection = (rightZ - leftZ)/(rightX - leftX);
            z = leftZ - zDirection;
            for(int i = leftX; i <= rightX; i++){
                if(zBuffer[lineInZBuffer + i] < z){
                    SDL_RenderDrawPoint(_renderer ,i, paintLine);
                    zBuffer[lineInZBuffer + i] = z;
                }
                z += zDirection;
            }
            //std::cout << z << "  ==  " << rightZ << "  " << z - rightZ << " smer: " << zDirection << "  --  " << (rightX - leftX) << std::endl;
        }
        else{
            zDirection = (leftZ - rightZ)/(leftX - rightX);
            z = rightZ + zDirection;
            for(int i = rightX; i <= leftX; i++)
                if(zBuffer[lineInZBuffer + i] < z){
                    SDL_RenderDrawPoint(_renderer ,i, paintLine);
                    zBuffer[lineInZBuffer + i] = z;
                }
                z += zDirection;
        }
        //Posun podle vypocitaneho smeru
        leftX += leftDirection;
        rightX += rightDirection;
        //Posun na dalsi radek
        paintLine ++;
        //Posun v hladine z pro dalsi radek
        leftZ += leftZDirection;
        rightZ += rightZDirection;
    }

    //std::cout << "paint half: " << leftZ << "  --  " << rightZ << std::endl;//"  --  " << rightPointLine << "  x  " << leftDirection << "  --  " << rightDirection << std::endl;
   //std::cout << "Left X: " << leftX << "  <=  " << rightX << std::endl << std::endl;

    // urceni ktereho bodu bylo dosazeno drive, upraveni smernice pro dalsi vykresleni
    if(paintLine >= leftPointLine){
        leftDirection = (right->x - left->x) / (right->y - left->y);
        leftX = left->x * zoom + viewPosX;
        leftZDirection = (right->z - left->z)/(rightPointLine - leftPointLine);
        leftZ = left->z * zoom;
    }
    else if(paintLine >= rightPointLine){
        rightDirection = (left->x - right->x) / (left->y - right->y);
        rightX = right->x * zoom + viewPosX;
        rightZDirection = ((left->z - right->z)*zoom)/(leftPointLine - rightPointLine);
        rightZ = right->z * zoom;
    }

    while (paintLine < leftPointLine || paintLine < rightPointLine){
        int lineInZBuffer = paintLine * width;
        float z, zDirection;

        if(leftX < rightX){
            zDirection = (rightZ - leftZ)/(rightX - leftX);
            z = leftZ + zDirection;
            for(int i = leftX; i <= rightX; i++){
                if(zBuffer[lineInZBuffer + i] < z){
                    SDL_RenderDrawPoint(_renderer ,i, paintLine);
                    zBuffer[lineInZBuffer + i] = z;
                }
                z += zDirection;
            }
        }
        else{
            zDirection = (leftZ - rightZ)/(leftX - rightX);
            z = rightZ + zDirection;
            for(int i = rightX; i <= leftX; i++)
                if(zBuffer[lineInZBuffer + i] < z){
                    SDL_RenderDrawPoint(_renderer ,i, paintLine);
                    zBuffer[lineInZBuffer + i] = z;
                }
                z += zDirection;
        }

        //Posun podle vypocitaneho smeru
        leftX += leftDirection;
        rightX += rightDirection;
        //Posun na dalsi radek
        paintLine ++;
        //Posun v hladine z pro dalsi radek
        leftZ += leftZDirection;
        rightZ += rightZDirection;
    }

    /*if(leftZ != rightZ){
        std::cout << "Error :" << leftZ << "  --  " << rightZ << std::endl;
        std::cout << "Contr :" << left->z * zoom << "  --  " << right->z * zoom << std::endl;
    }*/
}

void screen::Show(){
    //zobrazení vytvoøené plochy
    SDL_RenderPresent(_renderer);
}
