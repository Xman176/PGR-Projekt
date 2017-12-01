#ifndef RASTEROBJECT_H
#define RASTEROBJECT_H

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <string>
#include <fstream>

typedef struct point{
    float x;
    float y;
    float z;
    int r;
    int g;
    int b;
}point;

typedef struct trianglePoints{
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
    void RotateZ(float changeAngle);
    void Zoom(float zoomNumber);

    bool objectOK;
    float initZoom;
    bool colorObject;


private:
    int pointCount;
    int trianCount;
    trianglePoints *triangle;

    point *points;

    std::ifstream objectFile;

    void getPoints(int dataType, char *data);
    void getTriagles();

};



#endif // RASTEROBJECT_H
