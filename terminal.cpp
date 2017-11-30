#include "terminal.h"

void terminal::getFileName(){
    std::cout << "Zadejte nazev souboru:" << std::endl;
    getline(std::cin, fileName);
}

void terminal::writeError(char *errorMSG){
    std::cout << errorMSG << std::endl;
}
