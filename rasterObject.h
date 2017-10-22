#ifndef RASTEROBJECT_H
#define RASTEROBJECT_H

#include <iostream>
#include <cstdlib>
#include <math.h>

typedef struct point{
    float x;
    float y;
    float z;
}point;

typedef struct trianglePoints{
    //int countConnect;
    point *a;
    point *b;
    point *c;
}trianglePoints;

class rasterObject{
public:
    rasterObject();
    ~rasterObject();

    int GetTrianCount();
    trianglePoints* GetTriangl(int position);
    void GetNormal(trianglePoints* triangle, point* normalVec);


    void RotateX(float changeAngle);
    void RotateY(float changeAngle);


private:
    int pointCount;
    int trianCount;
    trianglePoints *triangle;

    point *points;

};



#endif // RASTEROBJECT_H
