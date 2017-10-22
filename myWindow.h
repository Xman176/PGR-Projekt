#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <iostream>
#include <SDL2/SDL.h>

#include "screen.h"

class myWindow{
public:
    myWindow(int, int);
    ~myWindow();


private:
    SDL_Window* _window;
    bool closeWindow, mouseClick;

    screen* _paintScreen;

    void Loop();
    void Rendering();
    void Input();
};

#endif
