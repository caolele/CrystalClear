//
//  GameMaster.cpp
//  CrystalClearKing
//
//  Created by Larry Cao on 16/12/18.
//  Copyright © 2016 King. All rights reserved.
//

#include <sstream>
#include "GameMaster.hpp"


GameMaster::GameMaster(RenderDelegate &rd):rd(rd){
    cmPtr = new CrystalMatrix(rd);
}

GameMaster::~GameMaster(){
    for(int i = 0; i < MATRIX_DIM * MATRIX_DIM; ++i){
        delete cmPtr->crystalMat[i];
    }
    delete cmPtr;
}

void GameMaster::run(){
    bool running = true;
    cmPtr->init();
    SDL_Event event;
    
    while(running){
        SDL_RenderClear(rd.ren);
        SDL_RenderCopy(rd.ren, rd.texBackground, NULL, NULL);
        SDL_RenderSetClipRect(rd.ren, &rd.crstMatBbox);
        
        unsigned int now = SDL_GetTicks();
        
        cmPtr->parseCrossClear(0);
        score += cmPtr->tryClear(now);
        cmPtr->reorganizeCrystals(now);
        
        cmPtr->draw(now);
        if(!isActive){
            SDL_Rect bbox = {MATRIX_TLPOS.first,
                MATRIX_TLPOS.second,
                CRYSTAL_DIM * MATRIX_DIM,
                CRYSTAL_DIM * MATRIX_DIM};
            SDL_SetRenderDrawColor(rd.ren, 125, 125, 125, 125);
            SDL_SetRenderDrawBlendMode(rd.ren, SDL_BLENDMODE_BLEND);
            SDL_RenderFillRect(rd.ren, &bbox);
            rd.renderText("Click to Start ...",
                          MATRIX_TLPOS.first + MATRIX_DIM / 3,
                          MATRIX_TLPOS.second + CRYSTAL_DIM * MATRIX_DIM / 3.5);
        }else if(now - timeBegin > MAX_TIME){
            isActive = false;
        }
      
        SDL_RenderSetClipRect(rd.ren, NULL);

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    HandelMouseBtnDown(now, &event);
                    break;
                case SDL_MOUSEBUTTONUP:
                    HandelMouseBtnUp(now, &event);
                    break;
                case SDL_MOUSEMOTION:
                    HandelMouseMove(now, &event);
                    break;
            }
        }
        
        updateTimeScore(now);
        SDL_RenderPresent(rd.ren);
    }
}

void GameMaster::HandelMouseBtnDown(unsigned int now, SDL_Event* event){
    if(isActive){
        cmPtr->hitCrstDownUp.first = cmPtr->tryHitCrystal(event->motion.x, event->motion.y);
    }else{
        isActive = true;
        timeBegin = now;
        return;
    }
}

void GameMaster::HandelMouseBtnUp(unsigned int now, SDL_Event* event){
    if(!isActive || !cmPtr->hitCrstDownUp.first) return;
    cmPtr->hitCrstDownUp.second = cmPtr->tryHitCrystal(event->motion.x, event->motion.y);
    
    if(cmPtr->hitCrstDownUp.first == cmPtr->hitCrstDownUp.second){
        Crystal* markedCrst = cmPtr->getMarkCrystal();
        if(!markedCrst){
            cmPtr->hitCrstDownUp.first->showMarker = true;
        }else{
            if(cmPtr->trySwapCrystals(now, cmPtr->hitCrstDownUp.second, markedCrst) < 1){
                markedCrst->showMarker = false;
                cmPtr->hitCrstDownUp.first->showMarker = true;
            }
        }
    }else{
        cmPtr->hitCrstDownUp.first->crstState = READY;
        cmPtr->trySwapCrystals(now, cmPtr->hitCrstDownUp.first, cmPtr->hitCrstDownUp.second, true);
    }
    cmPtr->hitCrstDownUp.first = nullptr;
}

void GameMaster::HandelMouseMove(unsigned int now, SDL_Event* event){
    if(!isActive || !cmPtr->hitCrstDownUp.first) return;
    Crystal* hover = cmPtr->tryHitCrystal(event->motion.x, event->motion.y);
    
    if(hover != cmPtr->hitCrstDownUp.first){
        if(hover){
            Crystal* markedCrst = cmPtr->getMarkCrystal();
            if(markedCrst) markedCrst->showMarker = false;
            hover->showMarker = true;
        }
        cmPtr->hitCrstDownUp.first->crstState = DRAG;
        cmPtr->hitCrstDownUp.first->xyHovPix = make_pair(event->motion.x, event->motion.y);
    }
}

void GameMaster::updateTimeScore(unsigned int now){
    if(!isActive){
        timeBegin = now;
        score = 0;
    }
    ostringstream timeSs, scoreSs;
    timeSs << "Time: "  << (int)((MAX_TIME - now + timeBegin)/1e3);
    rd.renderText(timeSs.str().c_str(), 25, 50);
    scoreSs << "Clear: "  << score;
    rd.renderText(scoreSs.str().c_str(), 25, 120);
}

