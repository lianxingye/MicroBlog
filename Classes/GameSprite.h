#ifndef __GAMESPRITE_H__
#define __GAMESPRITE_H__
#include "cocos2d.h"
using namespace cocos2d;
class GameSprite : public CCLabelTTF {
public:
	CC_SYNTHESIZE(CCPoint, _nextPosition, NextPosition);
	CC_SYNTHESIZE(CCPoint, _vector, Vector);
	CC_SYNTHESIZE(CCTouch *, _touch, Touch);
	GameSprite(void);
	~GameSprite(void);
	static GameSprite* gameSpriteWithFile(const char *
		pszFileName,  const char *pszLabelName);
    static GameSprite* gameSpriteWithFileFutureTime(const char *
        pszFileName, long futureTime);
	virtual void setPosition(const CCPoint& pos);
	float radius();
    
    void setStringLable(const char *pszLabelName);
    void setTimeLable(CCLabelTTF* lable);
    CCLabelTTF* getTimeLable();
    void setFutureTime(long time);
    long getFutureTime();
    time_t getFutureTimeFromUserFormatTimeString(CCString* ccb);
    
    time_t getCurrentTime();
    void itemTick();
    void setPicFileName(const char *pszFileName);
    const char * getPicFileName();
    void setInteval(long inteval);
    long getInteval();
    void resetClock();
private:
    CCLabelTTF* timeLabel;
    long futureTime;
    char fileName[30];
    long inteval;
    
    float delayTime;
    bool animationOn;
};
#endif // __GAMESPRITE_H__