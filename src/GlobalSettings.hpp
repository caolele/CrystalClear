//
//  GlobalSettings.hpp
//  CrystalClearKing
//
//  Created by Larry Cao on 16/12/18.
//

#ifndef GlobalSettings_hpp
#define GlobalSettings_hpp

#include <string>
#include <iostream>

using namespace std;


typedef pair<int, int> IntPair;

//Max time limit for each round of game
const int MAX_TIME = 60000;

//Application/window settings
const string APP_NAME = "The King of Crystal Clear";
const int WIN_W = 755;
const int WIN_H = 600;
const string TEXREF_BACKGROUND = "../Resources/BackGround.jpg";
const string FONT_REF = "../Resources/bbtn.ttf";


//Crystal settings
const std::string TEXREF_CRYSTAL[] = {
    "../Resources/Blue.png",
    "../Resources/Green.png",
    "../Resources/Purple.png",
    "../Resources/Red.png",
    "../Resources/Yellow.png"
};
const int CRYSTAL_TYPES = sizeof(TEXREF_CRYSTAL)/sizeof(TEXREF_CRYSTAL[0]);
const int CRYSTAL_DIM = 42;
typedef enum {
    READY, //standby & ready to perform further actions
    CLEAR, //is in the process of vanishing
    DRAG,  //is being dragged by mouse
    SWAP,  //is swapping with a neighbouring crystal
    FALL,  //is falling down to the closest vacant posisition
    DEAD   //the CLEAR action is done and ready to be recycled
} CRYSTAL_STATE;

//Crystal Matrix variables
const IntPair MATRIX_TLPOS = make_pair(330, 100); //top-left position of the crystal matrix
const int MATRIX_DIM = 8;

//Motion-control variable
const int TIME_CRYSTAL_SWAP = 200;
const int TIME_CRYSTAL_CLEAR = 200;



#endif /* GlobalSettings_h */
