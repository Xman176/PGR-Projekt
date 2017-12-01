#include "rasterObject.h"
#include "terminal.h"

rasterObject::rasterObject(){
    pointCount = 0;
    objectOK = true;
    colorObject = false;

    terminal terminalData;
    terminalData.getFileName();

    //prozatimni cteni ze souboru
    objectFile.open(terminalData.fileName);
    if(!objectFile.is_open()){
        terminalData.writeError(NOFILE);
        objectOK = false;
        return;
    }

    std::string line;

    //konrola typu souboru a typu ulozeni dat
    getline(objectFile, line);
    if(line.find("ply") == std::string::npos){
        terminalData.writeError("Nepodporovany obsah souboru. (Soubor musi zacinat radkem ply)");
        objectOK = false;
        return;
    }
    getline(objectFile, line);
    if(line.find("format ascii 1.0") == std::string::npos){
        terminalData.writeError("Nepodporovane formatovani souboru. (Podpora pouze ascii 1.0)");
        objectOK = false;
        return;
    }

    //promenne pro ziskana data z hlavicky (pozice bodu a barva) maximalne 6 hodnot
    int dataType = 0;
    char data[6];
    memset(data, 0, sizeof(char)*6);

    //ziskani informaci z hlavicky souboru
    while(getline(objectFile, line)){
        if(line.find("element vertex") != std::string::npos)
            pointCount = atoi(&line[14]); //14 = strlen(element vertex) prevadime cilo za textem
        else if(line.find("element face") != std::string::npos)
            trianCount = atoi(&line[12]); //12 = strlen(element face) prevadime cilo za textems
        else if((line.find("property float32") != std::string::npos)){
            if(line[18] == 0 && dataType < 6){
                data[dataType] = line[17];    //17 = strlen(element vertex) chceme ziskat znak za retezcem
                dataType ++;
            }
        }
        else if((line.find("property Uint8") != std::string::npos)){
            if(line[16] == 0 && dataType < 6){
                data[dataType] = line[15];    //15 = strlen(property Uint8) chceme ziskat znak za retezcem
                dataType ++;
            }
        }
        else if(line.find("end_header") != std::string::npos)
            break;
    }

    //zkontrolovani vsech barev
    bool r = false;
    bool g = false;
    bool b = false;
    for(int i = 0; i < dataType; i++){
        if(data[i] == 'r' || data[i] == 'R')
            r = true;
        if(data[i] == 'g' || data[i] == 'G')
            g = true;
        if(data[i] == 'b' || data[i] == 'B')
            b = true;
    }
    if(r && g && b)
        colorObject = true;

    getPoints(dataType, data);

    getTriagles();

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

void rasterObject::RotateZ(float changeAngle){
    float sinAngle = sin(changeAngle);
    float cosAngle = cos(changeAngle);

    for(int i = 0; i < pointCount; i++){
        float x = points[i].x;
        float y = points[i].y;

        points[i].x = x * cosAngle - y * sinAngle;
        points[i].y = y * cosAngle + x * sinAngle;
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

void rasterObject::getPoints(int dataType, char *data){
    std::string line;
    float minY = 0;
    float maxY = 0;

    points = new point[pointCount];
    for(int i = 0; i < pointCount; i++){
        getline(objectFile, line);
        int position = 0;

        for(int j = 0; j < dataType; j++){
            switch(data[j]){
                case 'x':
                case 'X':
                    points[i].x = atof(&line[position]);
                break;

                case 'y':
                case 'Y':
                    points[i].y = atof(&line[position]);
                    if(points[i].y < minY)
                        minY = points[i].y;
                    else if(points[i].y > maxY)
                        maxY = points[i].y;
                break;

                case 'z':
                case 'Z':
                    points[i].z = atof(&line[position]);
                break;

                case 'r':
                case 'R':
                    points[i].r = atoi(&line[position]);
                break;

                case 'g':
                case 'G':
                    points[i].g = atoi(&line[position]);
                break;

                case 'b':
                case 'B':
                    points[i].b = atoi(&line[position]);
                break;
            }
            for(; line[position ++] != ' ' && position < line.length(); )
                ;
        }
    }

    //Po nahrani vsech bodu proved priblizeni nebo oddaleni aby byl objekt videt
    initZoom = fabs(maxY) + fabs(minY);

}

void rasterObject::getTriagles(){
    std::string line;

    triangle = new trianglePoints[trianCount];
    for(int i = 0; i < trianCount; i++){
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
}

