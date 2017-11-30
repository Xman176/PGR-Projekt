#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <iostream>
#include <SDL2/SDL.h>

#include "screen.h"
#include "rasterObject.h"

class myWindow{
public:
    myWindow(int, int);
    ~myWindow();


private:
    SDL_Window* _window;
    bool closeWindow, leftMouse, rightMouse, repaint;

    screen* _paintScreen;

    void Loop();
    void Rendering();
    void Input();
};

#endif
