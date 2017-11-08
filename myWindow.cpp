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
    repaint = true;

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
        if(repaint)
            Rendering();

        Input();
    }

}

/*Funkce pro kontorlu vstupu
*/
void myWindow::Input(){
    SDL_Event ev;
    repaint = false;

    //Kontorla a dekodovani prichozi udalosti z mysi nebo klavesnice
    while(SDL_PollEvent(&ev) != 0){
        switch(ev.type){
            case SDL_QUIT: //Uzavreni okna pomoci krizku
                closeWindow = true;
                break;

            case SDL_WINDOWEVENT:
                if(ev.window.event == SDL_WINDOWEVENT_RESIZED){ //Udalost zmeny velikosti okna
                        _paintScreen->ScreenSize(ev.window.data1, ev.window.data2);
                        repaint = true;
                }
                break;

            case SDL_KEYDOWN: //Udalost zmacknute klavesy
                switch(ev.key.keysym.sym){
                    //Posun objektu
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

                    //Zmena rezimu vykreslovani
                    case SDLK_0:
                        _paintScreen->ChangePaintType(0);
                        break;
                    case SDLK_1:
                        _paintScreen->ChangePaintType(1);
                        break;

                    default:
                        break;
                }
                repaint = true;
                break;

            case SDL_MOUSEWHEEL: //Udalost otoceni koleckem pro zvetseni objektu
                if(ev.wheel.y == 1)
                    _paintScreen->Zoom(1);
                else if(ev.wheel.y == -1)
                    _paintScreen->Zoom(-1);

                repaint = true;
                break;

            case SDL_MOUSEMOTION: //Udalost pohybu mysi
                if(mouseClick){
                    _paintScreen->ChangeAngle(ev.motion.xrel, ev.motion.yrel);
                    repaint = true;
                }
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

}

void myWindow::Rendering(){

    _paintScreen->CleanRenderer();

    _paintScreen->PaintBackground(0, 0, 0, 255);

    _paintScreen->PaintObject();

    _paintScreen->Show();

}
