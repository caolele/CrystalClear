//
//  CrystalMatrix_hpp
//  CrystalClearKing
//
//  Created by Larry Cao on 16/12/18.
//  Copyright © 2016 King. All rights reserved.
//

#ifndef CrystalMatrix_hpp
#define CrystalMatrix_hpp

#include <random>
#include "RenderDelegate.hpp"
#include "GlobalSettings.hpp"
#include "Crystal.hpp"

using namespace std;


class CrystalMatrix{
    
private:
    
    RenderDelegate &rd;
    default_random_engine randNumGen;
    
    
public:
    
    // The actual data representation (1D array) for the entire crystal board
    Crystal* crystalMat[MATRIX_DIM * MATRIX_DIM];
    //To record the most-recent crystal hit by MouseDown and MouseUp (if any)
    pair<Crystal*, Crystal*> hitCrstDownUp = make_pair(nullptr, nullptr);
    
    CrystalMatrix(RenderDelegate &rd);
    
    //Initialize the matrix by calling init function of each crystal
    void init();
    
    //Obtain the 2D indexs (IntPair) from a 1D index (int)
    int getFlatIdx(IntPair xyIdx);
    
    //The master-loop to constantly rendering the entire matrix (play area)
    void draw(unsigned int now);
    
    //Return the crystal hit by location (xPix,yPix) if there's any
    Crystal* tryHitCrystal(int xPix, int yPix);
    
    //Return the first found marked crystal
    Crystal* getMarkCrystal();
    
    //Swap the location (on play area) of two crystals
    void swapCrystals(Crystal* crst1, Crystal* crst2);
    
    //Try to swap (both in memory and play-area) any two crystals (by either click or drag)
    unsigned int trySwapCrystals(unsigned int now, Crystal* crst1, Crystal* crst2, bool isDrag = false);
    
    //Recursive search crystal to clear from any 1D index position (left-to-right).
    //It will set the "toRemove" variable in each crystal object to true if it needs to be cleared.
    unsigned int parseCrossClear(int flatIdx);
    
    //Try to start the CLEAR animation for crystals with "toRemove==true"
    //When the CLEAR animation 
    unsigned int tryClear(unsigned int now);
    
    //Make crystals above vacancies fall & also recycle the DEAD to make new ones fall
    void reorganizeCrystals(unsigned int now);
    
    //Check if all crystals are in READY state
    bool allReady();
};


#endif /* CrystalMatrix_hpp */
