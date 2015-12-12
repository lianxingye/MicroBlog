//
//  RecordLayer.cpp
//  MicroBlog
//
//  Created by tosoth on 11/21/15.
//
//

#include "RecordLayer.h"

#define TAG_LABEL_LOG 1111

#define FONT_NAME                       "Thonburi"//"FZXS12--GB1-0.ttf"
//"FZXS12--GB1-0.ttf"
#define FONT_SIZE                       35

void RecordLayer::onEnter()
{
    CCLayer::onEnter();
    
    setAccelerometerEnabled(true);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //CCLabelTTF* logLabel = CCLabelTTF::create("logLabel", FONT_NAME, FONT_SIZE);
    //this->addChild(logLabel);
    //logLabel->setPosition(ccp(visibleSize.width/2, 100));
    
    //logLabel->setTag(TAG_LABEL_LOG);
    
    //scheduleUpdate();
}

void RecordLayer::onExit()
{
    setAccelerometerEnabled(false);
    CC_SAFE_DELETE(m_pCallFunc);
}

void RecordLayer::didAccelerate(CCAcceleration* pAccelerationValue)
{
    //CCCallFuncND;
    //CCLOG("x;%f y;%f z;%f", pAccelerationValue->x, pAccelerationValue->y , pAccelerationValue->z);
    
    CCLabelTTF* logLabel = (CCLabelTTF*)getChildByTag(TAG_LABEL_LOG);
    
    if (logLabel!=NULL) {
        
        float nowGX = (pAccelerationValue->x)*9.81f;
        float nowGY = (pAccelerationValue->y)*9.81f;
        if((nowGX<-10.0||nowGX>10.0)&&(nowGY<-10.0||nowGY>10.0))
        {
            //char str[255]={0};
            //sprintf(str, "value = %f", (nowGX>nowGY?nowGX:nowGY));
            //logLabel->setString(str);
            
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
