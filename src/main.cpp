//
//  main.cpp
//  CrystalClearKing
//
//  Created by Larry Cao on 16/12/18.
//

#include <iostream>
#include "GameMaster.hpp"


using namespace std;

int main(int argc, char **argv){
    try{
        RenderDelegate renderDelegate;
        GameMaster gameMaster(renderDelegate);
        gameMaster.run();
    }catch(char const* errMsg){
        cout << errMsg << endl;
        return 1;
    }
    
    return 0;
}

