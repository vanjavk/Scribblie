#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <iostream>
#include <vector>
using namespace std;

class context
{
public:
        

    const unsigned int width = 600;
    const unsigned int height = 400;
    int x = 0;
    int y = 0;
    int xrel = 0;
    int yrel = 0;
    vector<vector<array<int,2>>> points;
    int lineSize = 1;
    bool increase_lineSize();
    bool decrease_lineSize();
    //...
    int drawing = 0;
    SDL_Renderer *renderer;
    /**
     * Rectangle that the owl texture will be rendered into
     */
    SDL_Rect dest;
    SDL_Texture *owl_tex;
    /**
     * Font that is used for rendering text, and
     * a texture the text is rendered into
     */
    TTF_Font *font;
    SDL_Texture *text_tex;
    /**
     * x and y components of owl's velocity
     */
    int owl_vx;
    int owl_vy;

};