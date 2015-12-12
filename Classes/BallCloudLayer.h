//
//  BallCloudLayer.h
//  MicroBlog
//
//  Created by tosoth on 12/8/15.
//
//

#ifndef __MicroBlog__BallCloudLayer__
#define __MicroBlog__BallCloudLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class BallItem : public CCLabelTTF{
public:
    float x;
    float y;
    float z;
    float ele;
    
    BallItem(void);
    ~BallItem(void);
    
    static BallItem* createBallItem();
};

class BallCloudLayer : public CCLayer {
public:
    
    CREATE_FUNC(BallCloudLayer);
    virtual void onEnter();
    virtual void onExit();
    
    void rotateX();
    void rotateY();
    
    void hide();
    void go();
    
    void withPoint(float xx, float yy, float zz, BallItem* bi);
    
    float angleX;
    float angleY;
    
    int count;
    
    float coordinateForNormalizedValue(float normalizedValue, float rangeOffset);
    
    CCArray* ballGp;
    void update(float t);
    float secPassedSinceMidNight;
    
    void setStringArraySource(CCArray*);
    
    CCArray* stringArr;
};

#endif /* defined(__MicroBlog__BallCloudLayer__) */
