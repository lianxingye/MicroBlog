//
//  RecordLayer.cpp
//  MicroBlog
//
//  Created by tosoth on 11/21/15.
//
//

#include "RecordLayer.h"

#define TAG_LABEL_LOG 1111

#define FONT_NAME                       "FZXS12--GB1-0.ttf"
#define FONT_SIZE                       35


void RecordLayer::onEnter()
{
    CCLayer::onEnter();
    
    setAccelerometerEnabled(true);
    
    mm=0.0;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    
    CCLabelTTF* logLabel = CCLabelTTF::create("logLabel", FONT_NAME, FONT_SIZE);
    this->addChild(logLabel);
    logLabel->setPosition(ccp(visibleSize.width/2, 100));
    
    logLabel->setTag(TAG_LABEL_LOG);
    
    //scheduleUpdate();
}

void RecordLayer::onExit()
{
    CC_SAFE_DELETE(m_pCallFunc);
}


void RecordLayer::update(float t)
{
    CCLabelTTF* logLabel = (CCLabelTTF*)getChildByTag(TAG_LABEL_LOG);
    
    if (logLabel!=NULL) {
        mm+=0.01;
        char logString[40] = {0};
        sprintf(logString, "x;%f y;%f z;%f", 1.0, 2.0 , mm);
        logLabel->setString(logString);
        
        if (mm>0.5) {
            this->runAction(m_pCallFunc);
            mm=0;
        }
    }

}


void RecordLayer::didAccelerate(CCAcceleration* pAccelerationValue)
{
    //CCCallFuncND;
    
    CCLOG("x;%f y;%f z;%f", pAccelerationValue->x, pAccelerationValue->y , pAccelerationValue->z);
    
    CCLabelTTF* logLabel = (CCLabelTTF*)getChildByTag(TAG_LABEL_LOG);
    
    if (logLabel!=NULL) {
        
        float nowGX = (pAccelerationValue->x)*9.81f;
        float nowGY = (pAccelerationValue->y)*9.81f;
        if((nowGX<-10.0||nowGX>10.0)&&(nowGY<-10.0||nowGY>10.0))
        {
            char str[255]={0};
            sprintf(str, "value = %f", (nowGX>nowGY?nowGX:nowGY));
            logLabel->setString(str);
            
            // I dont know do what, I just know when I shake, do it
            go();
        }
    }
}

void RecordLayer::hide()
{
    this->setVisible(false);
}

void RecordLayer::go()
{
    if (m_pCallFunc) {
        m_pCallFunc->execute();
    }
}


void RecordLayer::setToBeCalledWhenShake(CCCallFunc* func)
{
    m_pCallFunc = func;
}

