#include "GameSprite.h"

#define FONT_NAME                       "Thonburi"
#define FONT_SIZE                       30

GameSprite::GameSprite(void)
{
	_vector = ccp(0, 0);
    timeLabel = NULL;
}
GameSprite::~GameSprite(void)
{
}
GameSprite* GameSprite::gameSpriteWithFile(const char *pszFileName, const char *pszLabelName)
{
    GameSprite * sprite = new GameSprite();
    if (sprite && sprite->initWithFile(pszFileName)) {
        sprite->setStringLable("----");
        
        long futureTime = sprite->getFutureTimeFromUserFormatTimeString(CCString::create(pszLabelName));
        long inteval = futureTime - sprite->getCurrentTime();
        sprite->setInteval(inteval);
        
        sprite->setFutureTime(futureTime);
        sprite->setPicFileName(pszFileName);
        
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}


GameSprite* GameSprite::gameSpriteWithFileFutureTime(const char *pszFileName, long futureTime)
{
    GameSprite * sprite = new GameSprite();
    if (sprite && sprite->initWithFile(pszFileName)) {
        sprite->setStringLable("----");
        /*
        long deltaTime = futureTime - sprite->getCurrentTime();
        int sec = deltaTime%60;
        int min = deltaTime/60%60;
        int hour = deltaTime/60/60%24;
        long day = deltaTime/60/60/24;
        
        char pszLabelName[40] = {0};
        sprintf(pszLabelName, "%ldd%dh%dm%ds", day, hour, min, sec);*/
        
        long inteval = futureTime - sprite->getCurrentTime();
        //sprite->setInteval(inteval);
        
        sprite->setFutureTime(futureTime);
        sprite->setPicFileName(pszFileName);
        
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void GameSprite::setInteval(long inteval)
{
    this->inteval = inteval;
}

long GameSprite::getInteval()
{
    return inteval;
}

void GameSprite::resetClock()
{
    this->futureTime = getCurrentTime() + getInteval();
    
    long deltaTime = getInteval();
    int sec = deltaTime%60;
    int min = deltaTime/60%60;
    int hour = deltaTime/60/60%24;
    long day = deltaTime/60/60/24;
    
    char pszLabelName[40] = {0};
    CCLOG("click resetClock on  %s", getPicFileName());
    
    if (day>0)
    {
        sprintf(pszLabelName, "%ldd",day);
    } else if (hour>0)
    {
        sprintf(pszLabelName, "%dd%dm",hour,min);
    } else
    {
        sprintf(pszLabelName, "%dm%ds",min,sec);
    }
    
    setStringLable(pszLabelName);
}

void GameSprite::setPicFileName(const char *pszFileName)
{
    for (int i = 0; i < sizeof(fileName); i++)
    {
        fileName[i] = 0;
    }
    strcpy(fileName, pszFileName);
}


const char * GameSprite::getPicFileName()
{
    return fileName;
}

void GameSprite::setPosition(const CCPoint& pos)
{
	CCSprite::setPosition(pos);
	if (!_nextPosition.equals(pos)) {
		_nextPosition = pos;
	}
}

void GameSprite::setStringLable(const char *pszLabelName)
{
    if (timeLabel != NULL)
    {
        timeLabel->setString(pszLabelName);
    } else
    {
        float ebikeWidth = this->getContentSize().width;
        float ebikeHeight = this->getContentSize().height;
        
        CCLabelTTF* ebiketime = CCLabelTTF::create(pszLabelName, FONT_NAME, FONT_SIZE);
        this->addChild(ebiketime);
        this->setTimeLable(ebiketime);
        
        float ebiketimeHeight = ebiketime->getContentSize().height;
        ebiketime->setPosition(ccp(ebikeWidth/2, ebikeHeight+ebiketimeHeight));
    }
}

void GameSprite::setTimeLable(CCLabelTTF* lable)
{
    if (timeLabel == NULL)
    {
        timeLabel = lable;
    }
}

void GameSprite::setFutureTime(long time)
{
    futureTime = time;
}

long GameSprite::getFutureTime()
{
    return futureTime;
}

void GameSprite::itemTick()
{
    long nowtime = getCurrentTime();
    long deltatime = futureTime - nowtime;
    
    long sec = deltatime % 60;
    long min = deltatime / 60 % 60;
    long hour = deltatime / 60 / 60 % 24;
    long day = deltatime / 60 / 60 / 24;
    
    if (deltatime >=0) {
        char mylable[40]={0};
        //sprintf(mylable, "%ldd%ldh%ldm%lds",day,hour,min,sec);
        if (day>0)
        {
            sprintf(mylable, "%ldd",day);
            if (day < 2) {
                sprintf(mylable, "%ldd%ldd",day,hour);
            }
        } else if (hour>0)
        {
            sprintf(mylable, "%ldd%ldm",hour,min);
        } else
        {
            sprintf(mylable, "%ldm%lds",min,sec);
        }
        setStringLable(mylable);
    } else {
        char mylable[40]={0};
        sprintf(mylable, "EXPIRE!");
        setStringLable(mylable);
    }
}

CCLabelTTF* GameSprite::getTimeLable()
{
    if (timeLabel == NULL)
    {
        return timeLabel;
    } else
    {
        return NULL;
    }
}

float GameSprite::radius() {
	return getTexture()->getContentSize().width * 0.5f;
}

time_t GameSprite::getFutureTimeFromUserFormatTimeString(CCString* ccb)
{
    time_t timep;
    
    char* origin = (char *)ccb->getCString();
    
    char* blank = origin+ccb->length()-1;
    
    CCString* eve_content = CCString::create("");
    
    if(strstr(origin," ") != NULL)
    {
        blank = strstr(origin," ");
        int from_start_to_blank_len = (int)(blank - origin);
        std::string anticipateTimeSec = ccb->m_sString.substr(0,from_start_to_blank_len);
        CCLOG("anticipateTimeSec %s", anticipateTimeSec.c_str());
        eve_content = CCString::create(ccb->m_sString.substr(from_start_to_blank_len,-1));
        CCLOG("eve_content %s", eve_content->getCString());
        ccb = CCString::create(anticipateTimeSec);
        origin = (char *)ccb->getCString();
    }
    char* d = origin;
    char* h = origin;
    char* m = origin;
    char* s = origin;
    
    // just put all the data into the saved position then it is ok
    //CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%dcontent", getCurrentLocation())->getCString(), eve_content->getCString());
    
    char *result_day_array = NULL;
    char *result_hour_array = NULL;
    char *result_min_array = NULL;
    char *result_sec_array = NULL;
    
    //CCLOG("---- - - - %s %s", strstr(origin,"d"), *blank);
    if(strstr(origin,"d") != NULL )// || strstr(a,"m") != NULL) // a long type or 3d5h3m1s
    {
        d = strstr(origin,"d");
        size_t len = d - origin;
        result_day_array = (char*)malloc(sizeof(char)*(len+1));
        strncpy(result_day_array, origin, len);
        result_day_array[len] = '\0';
    }
    if(strstr(origin,"h") != NULL )
    {
        
        h = strstr(origin,"h");
        size_t len;
        char* uplimit;
        if(strstr(origin,"d") != NULL)
        {
            uplimit = d+1;
            
        } else
        {
            uplimit = origin;
        }
        
        len = h - uplimit;
        
        result_hour_array = (char*)malloc(sizeof(char)*(len+1));
        strncpy(result_hour_array, uplimit, len);
        result_hour_array[len] = '\0';
    }
    if(strstr(origin,"m") != NULL )
    {
        
        m = strstr(origin,"m");
        size_t len;
        char* uplimit;
        if(strstr(origin,"h") != NULL)
        {
            uplimit = h+1;
            
        } else if(strstr(origin,"d") != NULL)
        {
            uplimit = d+1;
        } else
        {
            uplimit = origin;
        }
        
        len = m - uplimit;
        
        result_min_array = (char*)malloc(sizeof(char)*(len+1));
        strncpy(result_min_array, uplimit, len);
        result_min_array[len] = '\0';
    }
    if(strstr(origin,"s") != NULL )
    {
        
        s = strstr(origin,"s");
        size_t len;
        char* uplimit;
        if(strstr(origin,"m") != NULL)
        {
            uplimit = m+1;
            
        } else if(strstr(origin,"h") != NULL)
        {
            uplimit = h+1;
        } else if(strstr(origin,"d") != NULL)
        {
            uplimit = d+1;
        } else
        {
            uplimit = origin;
        }
        
        len = s - uplimit;
        
        result_sec_array = (char*)malloc(sizeof(char)*(len+1));
        
        strncpy(result_sec_array, uplimit, len);
        result_sec_array[len] = '\0';
    }
    
    int nDay=0, nHour=0, nMin=0, nSec=0;
    if(result_day_array!=NULL)nDay = atoi(result_day_array);
    if(result_hour_array!=NULL)nHour = atoi(result_hour_array);
    if(result_min_array!=NULL)nMin = atoi(result_min_array);
    if(result_sec_array!=NULL)nSec = atoi(result_sec_array);
    
    struct tm *tm;
    
    timep = getCurrentTime();
    tm = localtime(&timep);
    
    CCLOG("nowtime: %d %d:%d:%d",tm->tm_mday, tm->tm_hour ,tm->tm_min, tm->tm_sec);
    
    CCLOG("delta: %d %d:%d:%d", nDay, nHour , nMin, nSec);
    
    timep+=nSec;
    timep+=nMin*60;
    timep+=nHour*60*60;
    timep+=nDay*60*60*24;
    
    tm = localtime(&timep);
    
    CCLOG("futuretime: %d %d:%d:%d",tm->tm_mday, tm->tm_hour ,tm->tm_min, tm->tm_sec);
    
    return timep;
}


time_t GameSprite::getCurrentTime()
{
    time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    time(&timep);
#else
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    timep = now.tv_sec;
#endif
    return timep;
}
