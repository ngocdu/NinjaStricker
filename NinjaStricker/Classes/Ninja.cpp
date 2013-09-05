//
//  Ninja.cpp
//  TileGame
//
//  Created by Ngoc Du on 13/09/04.
//
//

#include "Ninja.h"
#include <cmath>
Ninja::Ninja()
{
    
}
void Ninja::actionMove(CCPoint point) {
    CCJumpTo* jumpAction;
    CCRotateBy* rotateAction;
    CCPoint p = this->getPosition();
    if (point.x > this->getPosition().x) {
        float height = (point.y - this->getPoint().y );
        int diffx = abs(point.x - this->getPoint().x);
        CCPoint p3 = CCPoint(this->getPoint().x + diffx * 2 , 0);
        float s =  caculatorDistance(this->getPoint(), point, p3);
        float time = s/this->getVelocity()/2;
        jumpAction = CCJumpTo::create(time, ccp(point.x + diffx, p.y), height , 1) ;
        rotateAction = CCRotateBy::create(1, 360 * time * 4);
    }else if (point.x < this->getPosition().x) {
        float height = (point.y - this->getPoint().y) ;
        int diffx = abs(point.x - this->getPoint().x);
        CCPoint p3 = CCPoint(this->getPoint().x + diffx * 2 , 0);
        float s =  caculatorDistance(this->getPoint(), point, p3);
        float time = s/this->getVelocity()/2;
        jumpAction = CCJumpTo::create(time, ccp(point.x - diffx, p.y), height , 1) ;
        rotateAction = CCRotateBy::create(1, 360 * time * 4);
    }
    
    jumpAction->setTag(1);
    this->runAction(jumpAction);
    this->runAction(rotateAction);
    
    
//    CCMoveTo * move = CCMoveTo::create(3, point);
//    this->runAction(move);
}
float Ninja::caculatorDistance(CCPoint p1, CCPoint p2, CCPoint p3) {
    float s = 0.0f;
    int s1 = ccpDistance(p1, p2);
    int s2 = ccpDistance(p3, p2);
    int s3 = ccpDistance(p1, p3);
    s = s1 + s2 + s3;
    return s;
}