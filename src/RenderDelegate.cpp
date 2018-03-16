//
//  RenderDelegate.cpp
//  CrystalClearKing
//
//  Created by Larry Cao on 16/12/18.
//  Copyright © 2016 King. All rights reserved.
//

#include <SDL2_image/SDL_image.h>
#include "RenderDelegate.hpp"
#include "GlobalSettings.hpp"

RenderDelegate::RenderDelegate(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) throw SDL_GetError();
    
    win = SDL_CreateWindow(APP_NAME.c_str(),
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           WIN_W, WIN_H, SDL_WINDOW_SHOWN);
    if(!win) throw SDL_GetError();
    
    ren = SDL_CreateRenderer(win, -1,
                             SDL_RENDERER_ACCELERATED |
                             SDL_RENDERER_PRESENTVSYNC);
    if(!ren) throw SDL_GetError();
    
    texBackground = IMG_LoadTexture(ren, TEXREF_BACKGROUND.c_str());
    if(!texBackground) throw SDL_GetError();
    
    for(int i = 0; i < CRYSTAL_TYPES; i++){
        SDL_Texture *tex = IMG_LoadTexture(ren, TEXREF_CRYSTAL[i].c_str());
        if(!tex) throw SDL_GetError();
        texCrystals.push_back(tex);
    }
    
    if(TTF_Init() != 0 || !(font=TTF_OpenFont(FONT_REF.c_str(), 30))){
        throw TTF_GetError();
    }
}

RenderDelegate::~RenderDelegate(){
    if(texBackground) SDL_DestroyTexture(texBackground);
    
    for (int i = 0; i < texCrystals.size(); i++)
        SDL_DestroyTexture(texCrystals[i]);
    texCrystals.clear();
    
    if(font) TTF_CloseFont(font);
    if(ren) SDL_DestroyRenderer(ren);
    if(win) SDL_DestroyWindow(win);
    
    SDL_Quit();
}

void RenderDelegate::renderTexture(int texId, IntPair xyPix, int d, bool isDrag, double scale){
    int tW, tH;
    SDL_Rect bbox;
    SDL_QueryTexture(texCrystals[texId], NULL, NULL, &tW, &tH);
    tW = (int)(tW * scale);
    tH = (int)(tH * scale);
    if(isDrag){
        bbox = {xyPix.first-tW/2, xyPix.second-tH/2, tW, tH};
    }else{
        bbox = {xyPix.first+(d-tW)/2, xyPix.second+(d-tH)/2, tW, tH};
    }
    SDL_RenderCopy(ren, texCrystals[texId], NULL, &bbox);
}

void RenderDelegate::renderText(const char *text, int x, int y){
    SDL_Color color = {255, 250, 250, 225};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    if(!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);
    if(!texture) return;
    SDL_FreeSurface(surface);
    SDL_Rect dst = {x, y, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}


