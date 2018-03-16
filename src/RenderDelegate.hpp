//
//  RenderDelegate.hpp
//  CrystalClear
//
//  Created by Larry Cao on 16/12/18.
//

#ifndef RenderDelegate_hpp
#define RenderDelegate_hpp

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include "GlobalSettings.hpp"


using namespace std;


class RenderDelegate{

public:

    SDL_Renderer *ren = nullptr;
    vector<SDL_Texture*> texCrystals;
    SDL_Texture* texBackground = nullptr;
    SDL_Window *win = nullptr;
    TTF_Font *font = nullptr;
    SDL_Rect crstMatBbox = {MATRIX_TLPOS.first,
                            MATRIX_TLPOS.second,
                            MATRIX_DIM * CRYSTAL_DIM,
                            MATRIX_DIM * CRYSTAL_DIM};

    RenderDelegate();
    ~RenderDelegate();

    void renderTexture(int texId, IntPair xyPix, int d, bool isDrag = false, double scale = 1);
    void renderText(const char *text, int x, int y);
};



#endif /* RenderDelegate_h */
