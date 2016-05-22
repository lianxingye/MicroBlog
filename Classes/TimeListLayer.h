//
//  TimeListLayer.hpp
//  MicroBlog
//
//  Created by tosoth on 5/18/16.
//
//

#ifndef TimeListLayer_hpp
#define TimeListLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameSprite.h"

#include "cJSON.h"

USING_NS_CC;

class TimeListLayer : public CCLayer {
public:
    
    TimeListLayer(void);
    ~TimeListLayer(void);
    CREATE_FUNC(TimeListLayer);
    virtual void onEnter();
    virtual void onExit();
    
    void UpdateAllLable();
    void addTimeItem(const char *szPicFileName, const char *szTimeFormat);
    void setCurrentLocation(int value);
    CCArray* splitEx(const std::string& src, std::string separate_character);
    CCArray* getTimeLableArray();
    
    CCArray* timeLableArray;
    int curLocation;
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
    bool saveContentToMemory(std::string ready_to_insert_content);

};



#endif /* TimeListLayer_hpp */
