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
//    CCJumpTo* jumpAction;
//    if (point.x > this->getPosition().x) {
//        float height = abs(point.y - this->getPosition().y + (point.y - this->getPosition().y) * 0.2f);
//        int diffx = abs(point.x - this->getPosition().x);
//        CCPoint p3 = CCPoint(this->getPosition().x + diffx * 2 + diffx * 0.2f, 0);
//        float s =  caculatorDistance(this->getPosition(), point, p3);
//        float time = s/this->getVelocity();
//        jumpAction = CCJumpTo::create(time, ccp(point.x, 0), height , 1) ;
//    }else if (point.x < this->getPosition().x) {
//        float height = abs(point.y - this->getPosition().y + (point.y - this->getPosition().y) * 0.2f);
//        int diffx = abs(point.x - this->getPosition().x);
//        CCPoint p3 = CCPoint(this->getPosition().x + diffx * 2 + diffx * 0.2f, 0);
//        float s =  caculatorDistance(this->getPosition(), point, p3);
//        float time = s/this->getVelocity();
//        jumpAction = CCJumpTo::create(time, ccp(point.x, 0), height , 1) ;
//    }
//    
//    CCRotateBy* rotateAction = CCRotateBy::create(1, 360);
//    this->runAction(jumpAction);
//    this->runAction(rotateAction);
    CCMoveTo * move = CCMoveTo::create(3, point);
    this->runAction(move);
}
float Ninja::caculatorDistance(CCPoint p1, CCPoint p2, CCPoint p3) {
    float s = 0.0f;
    int s1 = ccpDistance(p1, p2);
    int s2 = ccpDistance(p3, p2);
    int s3 = ccpDistance(p1, p3);
    s = s1 + s2 + s3;
    return s;
}