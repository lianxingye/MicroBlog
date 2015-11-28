//
//  RecordLayer.h
//  MicroBlog
//
//  Created by tosoth on 11/21/15.
//
//

#ifndef __MicroBlog__RecordLayer__
#define __MicroBlog__RecordLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class RecordLayer : public CCLayer {
public:
    
    CREATE_FUNC(RecordLayer);
    virtual void onEnter();
    virtual void onExit();
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
    
    void setToBeCalledWhenShake(CCCallFunc* func);
    
    void hide();
    void go();
    
protected:
    CCCallFunc*   m_pCallFunc;
};


#endif /* defined(__MicroBlog__RecordLayer__) */
