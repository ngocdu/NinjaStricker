#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <cmath>
using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    HudLayer *hud = new HudLayer();
    hud->init();
    scene->addChild(hud);
    layer->_hud = hud;
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("pickup.caf");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("hit.caf");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("move.caf");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("TileMap.caf");
    
    _tileMap = new CCTMXTiledMap();
    _tileMap->initWithTMXFile("TileMap.tmx");
    _background = _tileMap->layerNamed("Background");
    _foreground = _tileMap->layerNamed("Foreground");
    _foreground->getTexture()->setAntiAliasTexParameters();
    _meta = _tileMap->layerNamed("Meta");
    _meta->setVisible(false);
    
    
//    _tileMap->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(_tileMap);
    
    CCTMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Objects");
    
    if(objectGroup == NULL){
        CCLog("tile map has no objects object layer");
        return false;
    }
    
    CCDictionary *spawnPoint = objectGroup->objectNamed("SpawnPoint");
    
    int x = ((CCString)*spawnPoint->valueForKey("x")).intValue();
    int y = ((CCString)*spawnPoint->valueForKey("y")).intValue();
    
    _player = new Ninja();
    _player->setVelocity(250);
    _player->initWithFile("Player.png");
    _player->setPosition(ccp(x,y));
    
    this->addChild(_player);
    this->setViewPointCenter(_player->getPosition());
    
    this->setTouchEnabled(true);
    _moveAble = true;
    CCSprite *tile0 = _foreground->tileAt(ccp(5,10));
    CCSprite *tile1 = _foreground->tileAt(ccp(6,10));
    CCSprite *tile2 = _foreground->tileAt(ccp(7,10));
    CCMoveTo * move1 = CCMoveTo::create(3, ccp(15, 10));
    CCMoveTo * move2 = CCMoveTo::create(3, ccp(8, 10));
    CCSequence * sq = CCSequence::create(move1, move2, NULL);
    CCRepeatForever * repete0 = CCRepeatForever::create(sq);
    CCRepeatForever * repete1 = (CCRepeatForever*)(repete0->copy()->autorelease());
    CCRepeatForever * repete2 = (CCRepeatForever*)(repete0->copy()->autorelease());
    tile0->setAnchorPoint( ccp(0.5f, 0.5f) );
    tile1->setAnchorPoint( ccp(0.5f, 0.5f) );
    tile2->setAnchorPoint( ccp(0.5f, 0.5f) );
    tile0->runAction(repete0);
    tile1->runAction(repete1);
    tile2->runAction(repete2);
    
//    CCSprite *tile3 = _meta->tileAt(ccp(6,12));
//    CCMoveTo * move3 = CCMoveTo::create(5, ccp(800, 400));
//    tile3->runAction(move3);
    //----------------------
    this->schedule(schedule_selector(HelloWorld::update)) ;
    return true;
}

void HelloWorld::setViewPointCenter(CCPoint position)
{
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    int x = MAX(position.x, winSize.width/2);
    int y = MAX(position.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);
    
    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
    this->setPosition(viewPoint);
}

#pragma mark - handle touches

void HelloWorld::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool HelloWorld::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    _player->stopActionByTag(1);
    _player->setPosition(_player->getPoint());
    _moveAble = true;
    
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);
    
    CCSprite * t = CCSprite::create("Icon-72.png");
    CCScaleBy *scale = CCScaleBy::create(5, 0);
    CCHide *hide = CCHide::create();
    CCSequence * sq = CCSequence::create(scale, hide, NULL);
    t->runAction(sq);
    t->setPosition(touchLocation);
    this->addChild(t, 10);
    return true;
}

void HelloWorld::setPlayerPosition(CCPoint position)
{
    CCPoint tileCoord = this->tileCoordForPosition(position);
    int tileGid = _meta->tileGIDAt(tileCoord);
    if (tileGid) {
        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
        if (properties) {
            CCString *collision = new CCString();
            *collision = *properties->valueForKey("Collidable");
            if (collision && (collision->compare("True") == 0)) {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("hit.caf");
                _player->stopAllActions();
                _moveAble = false;
                return;
            }
            CCString *collectible = new CCString();
            *collectible = *properties->valueForKey("Collectable");
            if (collectible && (collectible->compare("True") == 0)) {
                _meta->removeTileAt(tileCoord);
                _foreground->removeTileAt(tileCoord);
                _numCollected++;
                _hud->numCollectedChanged(_numCollected);
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pickup.caf");
            }
        }
    }
//    _player->actionMove(position);
//    _player->setPosition(position);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("move.caf");
}

void HelloWorld::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);
    
    CCPoint playerPos = _player->getPosition();
    CCPoint diff = ccpSub(touchLocation, playerPos);
    
    if ( abs(diff.x) > abs(diff.y) ) {
        if (diff.x > 0) {
//            playerPos.x += _tileMap->getTileSize().width;
            playerPos.x += touchLocation.x - playerPos.x;
        } else {
//            playerPos.x -= _tileMap->getTileSize().width;
            playerPos.x -= - touchLocation.x + playerPos.x;
        }
    } else {
        if (diff.y > 0) {
//            playerPos.y += _tileMap->getTileSize().height;
            playerPos.y += touchLocation.y - playerPos.y;
        } else {
//            playerPos.y -= _tileMap->getTileSize().height;
            playerPos.y -= -touchLocation.y + playerPos.y;
        }
    }
    
    // safety check on the bounds of the map
    if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        playerPos.y >= 0 &&
        playerPos.x >= 0 )
    {
//        this->setPlayerPosition(playerPos);
        _player->actionMove(touchLocation);
    }
    
//    this->setViewPointCenter(_player->getPosition());
}

CCPoint HelloWorld::tileCoordForPosition(CCPoint position)
{
    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
    return ccp(x, y);
}
void HelloWorld::update(float dt) {
    _player->setPoint(_player->getPosition());
    this->setViewPointCenter(_player->getPosition());
    if (_moveAble == true) {
        this->setPlayerPosition(_player->getPosition());
    } 
}
