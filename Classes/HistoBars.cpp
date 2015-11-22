//
//  HistoBars.cpp
//  MicroBlog
//
//  Created by tosoth on 11/15/15.
//
//

#include "HistoBars.h"
HistoBarsSprite::HistoBarsSprite(void){
    _vector = ccp(0,0);
    _height = 1;
}

HistoBarsSprite::~HistoBarsSprite(void){
}

HistoBarsSprite* HistoBarsSprite::histoBarsSpriteWithFile(const char *
                                           pszFileName) {
    HistoBarsSprite * sprite = new HistoBarsSprite();
    if (sprite && sprite->initWithFile(pszFileName)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void HistoBarsSprite::setPosition(const CCPoint& pos) {
    CCSprite::setPosition(pos);
    if (!_nextPosition.equals(pos)) {
        _nextPosition = pos;
    }
}

float HistoBarsSprite::radius() {
    return getTexture()->getContentSize().width * 0.5f;
}