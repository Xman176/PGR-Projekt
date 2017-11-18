#include "rasterObject.h"

rasterObject::rasterObject(){
    pointCount = 0;

    std::ifstream objectFile;

    //prozatimni cteni ze souboru
    objectFile.open("turbine.ply");
    std::string line;

    while(getline(objectFile, line)){
        if(line.find("element vertex") != std::string::npos)
            pointCount = atoi(&line[14]);
        else if(line.find("element face") != std::string::npos)
            trianCount = atoi(&line[12]);
        else if(line.find("end_header") != std::string::npos)
            break;
    }

    int i;
    points = new point[pointCount];
    for(i = 0; i < pointCount; i++){
        getline(objectFile, line);
        int position = 0;
        points[i].x = atof(&line[position]);

        for(; line[position ++] != ' '; )
            ;

        points[i].y = atof(&line[position]);

        for(; line[position ++] != ' '; )
            ;

        points[i].z = atof(&line[position]);
    }

    triangle = new trianglePoints[trianCount];
    for(i = 0; i < trianCount; i++){
        getline(objectFile, line);
        if(line[0] == '4')
            std::cout << "error" << std::endl;

        int position = 0;
        int value = 0;

        for(; line[position ++] != ' '; )
            ;

        value = atoi(&line[position]);
        triangle[i].a = &points[value];

        for(; line[position ++] != ' '; )
            ;

        value = atoi(&line[position]);
        triangle[i].b = &points[value];

        for(; line[position ++] != ' '; )
            ;

        value = atoi(&line[position]);
        triangle[i].c = &points[value];
    }

    objectFile.close();
}

rasterObject::~rasterObject(){
    delete [] triangle;
    delete [] points;
}

int rasterObject::GetTrianCount(){
    return (trianCount);
}

trianglePoints* rasterObject::GetTriangl(int position){
    return (&triangle[position]);
}

void rasterObject::RotateY(float changeAngle){
    float sinAngle = sin(changeAngle);
    float cosAngle = cos(changeAngle);

    for(int i = 0; i < pointCount; i++){
        float y = points[i].y;
        float z = points[i].z;

        points[i].y = y * cosAngle - z * sinAngle;
        points[i].z = z * cosAngle + y * sinAngle;
    }

}

void rasterObject::RotateX(float changeAngle){
    float sinAngle = sin(changeAngle);
    float cosAngle = cos(changeAngle);

    for(int i = 0; i < pointCount; i++){
        float x = points[i].x;
        float z = points[i].z;

        points[i].x = x * cosAngle - z * sinAngle;
        points[i].z = z * cosAngle + x * sinAngle;
    }

}

void rasterObject::Zoom(float zoomNumber){
    for(int i = 0; i < pointCount; i++){
        points[i].x *= zoomNumber;
        points[i].y *= zoomNumber;
        points[i].z *= zoomNumber;
    }
}

void rasterObject::GetNormal(trianglePoints* triangle, point* normalVec){
    point uVec, vVec;

    //Vypocet vektoru U mezi body B a A
    uVec.x = triangle->b->x - triangle->a->x;
    uVec.y = triangle->b->y - triangle->a->y;
    uVec.z = triangle->b->z - triangle->a->z;

    //Vypocet vektoru V mezi body B a C
    vVec.x = triangle->b->x - triangle->c->x;
    vVec.y = triangle->b->y - triangle->c->y;
    vVec.z = triangle->b->z - triangle->c->z;

    //Vypocet vektrou normaly pro trojuhelnik
    normalVec->x = uVec.y * vVec.z - uVec.z * vVec.y;
    normalVec->y = uVec.z * vVec.x - uVec.x * vVec.z;
    normalVec->z = uVec.x * vVec.y - uVec.y * vVec.x;
}



