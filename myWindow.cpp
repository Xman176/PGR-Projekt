#include "myWindow.h"

/*
** Konstruktor okna pro rasterizace
** param1: sirka okna
** param2: vyska okna
*/
myWindow::myWindow(int width, int height) : _paintScreen(nullptr){
    SDL_Renderer* _render;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "SDL init error:" << SDL_GetError() << std::endl;
    }

    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &_window, &_render);
    SDL_SetWindowTitle(_window, "PGR Projekt Rasterizace");

    closeWindow = false;
    mouseClick = false;

    _paintScreen = new screen(_render, width, height);
    //_paintScreen.SetRenderer();

    Loop();
}


myWindow::~myWindow(){

    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void myWindow::Loop(){

    while(!closeWindow){
        Rendering();
        Input();
    }

}

void myWindow::Input(){
    SDL_Event ev;

    while(SDL_PollEvent(&ev) != 0){
        switch(ev.type){
            case SDL_QUIT:
                closeWindow = true;
                break;

            case SDL_WINDOWEVENT:
                if(ev.window.event == SDL_WINDOWEVENT_RESIZED)
                        _paintScreen->ScreenSize(ev.window.data1, ev.window.data2);
                break;

            case SDL_KEYDOWN:
                switch(ev.key.keysym.sym){
                    case SDLK_LEFT:
                        _paintScreen->Move(-1, 0);
                        break;
                    case SDLK_RIGHT:
                        _paintScreen->Move(1, 0);
                        break;
                    case SDLK_UP:
                        _paintScreen->Move(0, -1);
                        break;
                    case SDLK_DOWN:
                        _paintScreen->Move(0, 1);
                        break;
                    default:
                        break;
                }
                break;

            case SDL_MOUSEWHEEL:
                if(ev.wheel.y == 1)
                    _paintScreen->Zoom(1);
                else if(ev.wheel.y == -1)
                    _paintScreen->Zoom(-1);
                break;

            case SDL_MOUSEMOTION:
                if(mouseClick)
                    _paintScreen->ChangeAngle(ev.motion.xrel, ev.motion.yrel);
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(ev.button.button == SDL_BUTTON_LEFT)
                    mouseClick = true;
                break;

            case SDL_MOUSEBUTTONUP:
                if(ev.button.button == SDL_BUTTON_LEFT)
                    mouseClick = false;
                break;
        }
    }

    /*std::cout << "Event type num:" << ev.type << "  ---  " << ev.text.text;
    std::cin.ignore();*/

}

void myWindow::Rendering(){

    _paintScreen->CleanRenderer();

    _paintScreen->PaintBackground(0, 0, 0, 255);

    _paintScreen->PaintObject();

    _paintScreen->Show();

}
