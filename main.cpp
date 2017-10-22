#include "rasterObject.h"
#include "screen.h"
#include "myWindow.h"



int main(int argc, char *argv[]){

    //rasterObject myObject;

    myWindow myWin(640,480);


/*    SDL_Window *window = nullptr;



    window = SDL_CreateWindow("PGR Projekt Rasterizace", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if(window == NULL){
        std::cout << "SDL create window error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    bool closeWindow = false;
    SDL_Event ev;

    while(!closeWindow){
        while(SDL_PollEvent(&ev) != 0){
            switch(ev.type){
                case SDL_QUIT:
                    closeWindow = true;
                    break;
                case SDL_WINDOWEVENT:
                    if(ev.window.event == SDL_WINDOWEVENT_RESIZED)
                        std::cout << "New size: " << ev.window.data1 << "  x  " << ev.window.data2 << std::endl;
                    break;
            }

        }

        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

*/
    return EXIT_SUCCESS;
}
