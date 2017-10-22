#include "rasterObject.h"

rasterObject::rasterObject(){
 /*   pointCount = 12;

    points = new point[pointCount];

    points[0] = {0, -0.525731, 0.850651};
    points[1] = {0.850651, 0, 0.525731};
    points[2] = {0.850651, 0, -0.525731};
    points[3] = {-0.850651, 0, -0.525731};
    points[4] = {-0.850651, 0, 0.525731};
    points[5] = {-0.525731, 0.850651, 0};
    points[6] = {0.525731, 0.850651, 0};
    points[7] = {0.525731, -0.850651, 0};
    points[8] = {-0.525731, -0.850651, 0};
    points[9] = {0, -0.525731, -0.850651};
    points[10] = {0, 0.525731, -0.850651};
    points[11] = {0, 0.525731, 0.850651};

    trianCount = 20;

    triangle = new trianglePoints[trianCount];
    triangle[0] = {&points[6], &points[2], &points[1]};
    triangle[1] = {&points[2], &points[7], &points[1]};
    triangle[2] = {&points[5], &points[4], &points[3]};
    triangle[3] = {&points[8], &points[3], &points[4]};
    triangle[4] = {&points[11], &points[5], &points[6]};
    triangle[5] = {&points[10], &points[6], &points[5]};
    triangle[6] = {&points[2], &points[10], &points[9]};
    triangle[7] = {&points[3], &points[9], &points[10]};
    triangle[8] = {&points[9], &points[8], &points[7]};
    triangle[9] = {&points[0], &points[7], &points[8]};
    triangle[10] = {&points[1], &points[0], &points[11]};
    triangle[11] = {&points[4], &points[11], &points[0]};
    triangle[12] = {&points[10], &points[2], &points[6]};
    triangle[13] = {&points[11], &points[6], &points[1]};
    triangle[14] = {&points[10], &points[5], &points[3]};
    triangle[15] = {&points[11], &points[4], &points[5]};
    triangle[16] = {&points[9], &points[7], &points[2]};
    triangle[17] = {&points[0], &points[1], &points[7]};
    triangle[18] = {&points[8], &points[9], &points[3]};
    triangle[19] = {&points[0], &points[8], &points[4]};
*/
    pointCount = 6;

    points = new point[pointCount];

    points[0] = {1, 0, 0};
    points[1] = {0, -1, 0};
    points[2] = {-1, 0, 0};
    points[3] = {0, 1, 0};
    points[4] = {0, 0, 1};
    points[5] = {0, 0, -1};

    trianCount = 8;

    triangle = new trianglePoints[trianCount];
    triangle[0] = {&points[4], &points[0], &points[1]};
    triangle[1] = {&points[4], &points[1], &points[2]};
    triangle[2] = {&points[4], &points[2], &points[3]};
    triangle[3] = {&points[4], &points[3], &points[0]};
    triangle[4] = {&points[5], &points[1], &points[0]};
    triangle[5] = {&points[5], &points[2], &points[1]};
    triangle[6] = {&points[5], &points[3], &points[2]};
    triangle[7] = {&points[5], &points[0], &points[3]};


   // std::cout << "Value: " << points[6].x << " -- " << points[6].y << " -- " << points[6].z << std::endl;
   // std::cout << "Trian: " << triangle[0].a.x << " -- " << triangle[0].a.y << " -- " << triangle[0].a.z << std::endl;
   //std::cout << "Count: " << GetTrianCount() << std::endl;
/*
   pointCount = 3;

   points = new point[pointCount];

    points[0] = {0, -1, 0};
    points[1] = {1, 2, 0};
    points[2] = {-1, -2, 0};

        trianCount = 1;

    triangle = new trianglePoints[trianCount];
    triangle[0] = {&points[0], &points[1], &points[2]};*/
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



