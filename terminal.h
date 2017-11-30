#ifndef TERMINAL_H
#define TERMINAL_H

#define NOFILE "Soubor neexistuje!"

#include <iostream>
#include <cstring>

class terminal{
public:
    std::string fileName;
    void getFileName();
    void writeError(char *errorMSG);
};


#endif // TERMINAL_H
