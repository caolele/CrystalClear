//
//  GameMaster.hpp
//  CrystalClearKing
//
//  Created by Larry Cao on 16/12/18.
//

#ifndef GameMaster_hpp
#define GameMaster_hpp

#include "RenderDelegate.hpp"
#include "CrystalMatrix.hpp"

class GameMaster{
    
private:
    
    RenderDelegate	&rd;
    CrystalMatrix* cmPtr = nullptr;
    bool isActive = false;
    unsigned int score = 0;
    unsigned int timeBegin = 0;
  
    
public:
    
    GameMaster(RenderDelegate &r);
    ~GameMaster();
    void run();
    
    void HandelMouseBtnDown(unsigned int now, SDL_Event* event);
    void HandelMouseBtnUp(unsigned int now, SDL_Event* event);
    void HandelMouseMove(unsigned int now, SDL_Event* event);
    void updateTimeScore(unsigned int now);
};


#endif /* GameMaster_hpp */
