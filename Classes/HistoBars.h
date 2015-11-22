//
//  HistoBars.h
//  MicroBlog
//
//  Created by tosoth on 11/15/15.
//
//

#ifndef __MicroBlog__HistoBars__
#define __MicroBlog__HistoBars__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class HistoBarsSprite : public CCSprite {
public:
    CC_SYNTHESIZE(CCPoint, _nextPosition, NextPosition);
    CC_SYNTHESIZE(CCPoint, _vector, Vector);
    CC_SYNTHESIZE(CCTouch *, _touch, Touch);
    CC_SYNTHESIZE(int, _height, MyHeight);
    
    HistoBarsSprite(void);
    ~HistoBarsSprite(void);
    
    static HistoBarsSprite* histoBarsSpriteWithFile(const char *
                                          pszFileName);
    virtual void setPosition(const CCPoint& pos);
    float radius();
    
};



#endif /* defined(__MicroBlog__HistoBars__) */
