//
//  CrystalMatrix.cpp
//  CrystalClearKing
//
//  Created by Larry Cao on 16/12/18.
//  Copyright © 2016 King. All rights reserved.
//

#include "CrystalMatrix.hpp"


CrystalMatrix::CrystalMatrix(RenderDelegate &rd):rd(rd){
    randNumGen.seed((unsigned int)time(0));
}

void CrystalMatrix::init(){
    uniform_int_distribution<int> crst_uniform_dist(0, CRYSTAL_TYPES - 1);
    for(int i = 0; i < MATRIX_DIM * MATRIX_DIM; ++i){
        Crystal* crst = new Crystal(rd);
        crst->init(i, crst_uniform_dist(randNumGen));
        crystalMat[i] = crst;
    }
}

int CrystalMatrix::getFlatIdx(IntPair xyIdx){
    return xyIdx.second * MATRIX_DIM + xyIdx.first;
}

void CrystalMatrix::draw(unsigned int now){
    for(int i = 0; i < MATRIX_DIM * MATRIX_DIM; ++i){
        crystalMat[i]->draw(now);
    }
    //Make sure dragging crystal is on top
    for(int i = 0; i < MATRIX_DIM * MATRIX_DIM; ++i){
        if(crystalMat[i]->crstState == DRAG)
            crystalMat[i]->draw(now);
    }
}


Crystal* CrystalMatrix::tryHitCrystal(int xPix, int yPix){
    for(int i = 0; i < MATRIX_DIM * MATRIX_DIM; ++i){
        if(!crystalMat[i]) continue;
        if(crystalMat[i]->isHit(xPix, yPix) &&
           crystalMat[i]->crstState == READY){
            return crystalMat[i];
        }
    }
    return nullptr;
}

Crystal* CrystalMatrix::getMarkCrystal(){
    for(int i = 0; i < MATRIX_DIM * MATRIX_DIM; ++i){
        if(!crystalMat[i]) continue;
        if(crystalMat[i]->showMarker) return crystalMat[i];
    }
    return nullptr;
}

void CrystalMatrix::swapCrystals(Crystal* crst1, Crystal* crst2){
    swap(crystalMat[getFlatIdx(crst1->xyIdx)],
         crystalMat[getFlatIdx(crst2->xyIdx)]);
    crst1->xyPrevIdx = crst1->xyIdx;
    crst1->xyIdx = crst2->xyIdx;
    crst2->xyPrevIdx = crst2->xyIdx;
    crst2->xyIdx = crst1->xyPrevIdx;
}

unsigned int CrystalMatrix::trySwapCrystals(unsigned int now, Crystal* crst1, Crystal* crst2, bool isDrag){
    if(!crst1 || !crst2 ||
       crst1->crstState != READY ||
       crst2->crstState != READY ||
       !crst1->isAdjacent(crst2)){
        return 0;
    }
    
    swapCrystals(crst1, crst2);
    
    unsigned int numToClear = parseCrossClear(0);
    if(numToClear < 1){
        swapCrystals(crst1, crst2);
        return numToClear;
    }
    
    crst1->showMarker = crst2->showMarker = false;
    crst1->timeMoveBegin = crst2->timeMoveBegin = now;
    if(!isDrag) crst1->crstState = SWAP;
    crst2->crstState = SWAP;
    
    return numToClear;
}

unsigned int CrystalMatrix::parseCrossClear(int curIdx){
    int count = 0;
    if(curIdx < 0 || curIdx >= MATRIX_DIM * MATRIX_DIM)
        return count;
    
    count += parseCrossClear(curIdx+1);
    
    if(crystalMat[curIdx]->crstState != READY) return count;
    
    int x = crystalMat[curIdx]->xyIdx.first;
    int y = crystalMat[curIdx]->xyIdx.second;
    
    if(x-1 >= 0 && x+1 < MATRIX_DIM){
        Crystal* leftCrst = crystalMat[getFlatIdx(make_pair(x-1,y))];
        Crystal* rightCrst = crystalMat[getFlatIdx(make_pair(x+1,y))];
        if(leftCrst->texID == crystalMat[curIdx]->texID &&
           rightCrst->texID == crystalMat[curIdx]->texID){
            if(!leftCrst->toRemove && leftCrst->crstState==READY){
                leftCrst->toRemove = true;
                count++;
            }
            if(!rightCrst->toRemove && rightCrst->crstState==READY){
                rightCrst->toRemove = true;
                count++;
            }
            if(!crystalMat[curIdx]->toRemove){
                crystalMat[curIdx]->toRemove = true;
                count++;
            }
        }
    }
    
    if(y-1 >= 0 && y+1 < MATRIX_DIM){
        Crystal* bottomCrst = crystalMat[getFlatIdx(make_pair(x,y-1))];
        Crystal* topCrst = crystalMat[getFlatIdx(make_pair(x,y+1))];
        if(bottomCrst->texID == crystalMat[curIdx]->texID &&
           topCrst->texID == crystalMat[curIdx]->texID){
            if(!bottomCrst->toRemove && bottomCrst->crstState==READY){
                bottomCrst->toRemove = true;
                count++;
            }
            if(!topCrst->toRemove && topCrst->crstState==READY){
                topCrst->toRemove = true;
                count++;
            }
            if(!crystalMat[curIdx]->toRemove){
                crystalMat[curIdx]->toRemove = true;
                count++;
            }
        }
    }
    
    return count;
}

unsigned int CrystalMatrix::tryClear(unsigned int now){
    unsigned int numCleared = 0;
    for(int i = 0; i < MATRIX_DIM * MATRIX_DIM; ++i){
        if(crystalMat[i]->toRemove &&
           crystalMat[i]->crstState == READY){
             crystalMat[i]->showMarker = false;
            crystalMat[i]->timeClearBegin = now;
            crystalMat[i]->crstState = CLEAR;
            crystalMat[i]->toRemove = false;
            numCleared++;
        }
    }
    return numCleared;
}

void CrystalMatrix::reorganizeCrystals(unsigned int now){
    for(int i = MATRIX_DIM * MATRIX_DIM - 1; i >= 0; --i){
        if(crystalMat[i]->crstState == DEAD){
            for(int j = i-MATRIX_DIM; j >= 0; j-=MATRIX_DIM){
                if(crystalMat[j]->crstState == READY){
                    crystalMat[j]->startFall(now, crystalMat[i]);
                    swap(crystalMat[i], crystalMat[j]);
                    break;
                }
            }
        }
    }
    
    if(!allReady()) return;
    //Recycle and reuse dead crystals
    uniform_int_distribution<int> crst_uniform_dist(0, CRYSTAL_TYPES - 1);
    for(int i = MATRIX_DIM * MATRIX_DIM - 1; i >= 0; --i){
        if(crystalMat[i]->crstState == DEAD){
            crystalMat[i]->init(i, crst_uniform_dist(randNumGen));
            crystalMat[i]->timeFallBegin = now;
            crystalMat[i]->crstState = FALL;
            crystalMat[i]->xyPrevIdx = make_pair(i%MATRIX_DIM, i/MATRIX_DIM - 1);
        }
    }
}

bool CrystalMatrix::allReady(){
    for(int i = 0; i < MATRIX_DIM * MATRIX_DIM; ++i){
        if(crystalMat[i]->crstState != READY && crystalMat[i]->crstState != DEAD){
            return false;
        }
    }
    return true;
}


