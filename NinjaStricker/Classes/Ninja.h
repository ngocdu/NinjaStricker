//
//  Ninja.h
//  TileGame
//
//  Created by Ngoc Du on 13/09/04.
//
//

#ifndef TileGame_Ninja_h
#define TileGame_Ninja_h
#include "cocos2d.h"
#include <string.h>
#include "SimpleAudioEngine.h"
using namespace std;
using namespace cocos2d;
class Ninja : public CCSprite
{
private:
    CC_SYNTHESIZE(string, _name, Name);
    CC_SYNTHESIZE(int, _id, Id);
    CC_SYNTHESIZE(float, _timeAction, TimeAction);
    CC_SYNTHESIZE(int, _catagoryFruit, CatagoryFruit);
    CC_SYNTHESIZE(CCPoint, _point, Point);
    CC_SYNTHESIZE(int, _velocity, Velocity);
public:
	Ninja();
//	Ninja();// ham tao co doi so
    void actionMove(cocos2d::CCPoint point);
    float caculatorDistance(CCPoint p1, CCPoint p2, CCPoint p3);
};


#endif
