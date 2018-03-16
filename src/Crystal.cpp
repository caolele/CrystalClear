//
//  Crystal.cpp
//  CrystalClear
//
//  Created by Larry Cao on 16/12/18.
//

#include "Crystal.hpp"

Crystal::Crystal(RenderDelegate &rd):rd(rd){}

IntPair Crystal::index2Pixel(IntPair srcIdxPair){
    return make_pair(MATRIX_TLPOS.first + srcIdxPair.first * CRYSTAL_DIM,
                     MATRIX_TLPOS.second + srcIdxPair.second * CRYSTAL_DIM);
}

void Crystal::init(int idx, int texId){
    // fold 1D array to 2D matrix
    xyIdx = make_pair(idx%MATRIX_DIM, idx/MATRIX_DIM);
    crstState = READY;
    texID = texId;
}

void Crystal::draw(unsigned int now){
    if(crstState == READY){
        IntPair xyPix = index2Pixel(xyIdx);
        rd.renderTexture(texID, xyPix, CRYSTAL_DIM);
    }else if(crstState == SWAP){
        if(now > timeMoveBegin + TIME_CRYSTAL_SWAP){
            crstState = READY;
        }else{
            IntPair xyPixDst = index2Pixel(xyIdx);
            IntPair xyPixSrc = index2Pixel(xyPrevIdx);
            double c = (double)(now - timeMoveBegin) / TIME_CRYSTAL_SWAP;
            IntPair xyPix = make_pair((int)(c*(xyPixDst.first-xyPixSrc.first)+xyPixSrc.first),
                                      (int)(c*(xyPixDst.second-xyPixSrc.second)+xyPixSrc.second));
            rd.renderTexture(texID, xyPix, CRYSTAL_DIM);
        }
    }else if(crstState == DRAG){
        rd.renderTexture(texID, xyHovPix, CRYSTAL_DIM, true, 1.1);
    }else if(crstState == CLEAR){
        if(now > timeClearBegin + TIME_CRYSTAL_CLEAR){
            crstState = DEAD;
        }
        IntPair xyPix = index2Pixel(xyIdx);
        double scale = 1.0 - (double)(now-timeClearBegin)/TIME_CRYSTAL_CLEAR;
        rd.renderTexture(texID, xyPix, CRYSTAL_DIM, false, scale);
    }else if(crstState == FALL){
        IntPair xyPix = index2Pixel(xyIdx);
        double sec = (double)(now-timeFallBegin)/1e3;
        int yPixNew = (int)(index2Pixel(xyPrevIdx).second + 20*sec*sec*CRYSTAL_DIM);
        if(yPixNew > xyPix.second){
            crstState = READY;
        }else{
            rd.renderTexture(texID, make_pair(xyPix.first, yPixNew), CRYSTAL_DIM);
        }
    }
    
    if(showMarker){
        IntPair xyPix = index2Pixel(xyIdx);
        SDL_SetRenderDrawColor(rd.ren, 255, 255, 255, 255);
        SDL_SetRenderDrawBlendMode(rd.ren, SDL_BLENDMODE_BLEND);
        SDL_Rect bbox = {xyPix.first, xyPix.second, CRYSTAL_DIM, CRYSTAL_DIM};
        SDL_RenderDrawRect(rd.ren, &bbox);
    }
}

bool Crystal::isHit(int xPix, int yPix){
    IntPair xyPix = index2Pixel(xyIdx);
    if(xPix > xyPix.first && xPix < xyPix.first + CRYSTAL_DIM &&
       yPix > xyPix.second && yPix < xyPix.second + CRYSTAL_DIM){
        return true;
    }else return false;
}

bool Crystal::isAdjacent(Crystal* crst){
    int xDif = abs(xyIdx.first - crst->xyIdx.first);
    int yDif = abs(xyIdx.second - crst->xyIdx.second);
    return (xDif+yDif == 1);
}

void Crystal::startFall(unsigned int now, Crystal* dst){
    showMarker = false;
    timeFallBegin = now;
    crstState = FALL;
    xyPrevIdx = xyIdx;
    xyIdx = dst->xyIdx;
    dst->xyPrevIdx = dst->xyIdx;
    dst->xyIdx = xyPrevIdx;
}
