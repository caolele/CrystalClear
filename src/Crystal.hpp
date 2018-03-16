//
//  Crystal.hpp
//  CrystalClear
//
//  Created by Larry Cao on 16/12/18.
//

#ifndef Crystal_hpp
#define Crystal_hpp

#include "GlobalSettings.hpp"
#include "RenderDelegate.hpp"

using namespace std;


class Crystal{
    
private:
    RenderDelegate &rd;
    
public:
    
    //The state of this crystal
    CRYSTAL_STATE crstState = READY;
    //Is this crystal selected/marked?
    bool showMarker = false;
    //The 2D index pair
    IntPair xyIdx = make_pair(0, 0);
    //From which positon (2D index) does this crystal move from?
    IntPair xyPrevIdx = make_pair(0, 0);
    //Used when it's being dragged
    IntPair xyHovPix = make_pair(0, 0);
    //Crystal type (a.k.a. texture type)
    int texID = 0;
    //Please clear this crystal A.S.A.P. when possible
    bool toRemove = false;
    
    //allow some time for animating move/clear/fall action from the following time point.
    unsigned int timeMoveBegin = 0;
    unsigned int timeClearBegin = 0;
    unsigned int timeFallBegin = 0;
    
    Crystal(RenderDelegate &rd);
    IntPair index2Pixel(IntPair srcIdxPair);
    
    //Initialize this crystal with a random type using normal distribution
    void init(int idx, int texId);
    
    //Render this crystal according to "crstState"; called by "draw" method in CrystalMatrix obj.
    void draw(unsigned int now);
    
    //Is pixel point (xPix, yPix) lies on this crystal?
    bool isHit(int xPix, int yPix);
    
    //Is Crystal* crst next to me??
    bool isAdjacent(Crystal* crst);
    
    //Let me fall down to the position of dst
    void startFall(unsigned int now, Crystal* dst);
    
};

#endif /* Crystal_hpp */
