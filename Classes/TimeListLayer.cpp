//
//  TimeListLayer.cpp
//  MicroBlog
//
//  Created by tosoth on 5/18/16.
//
//

#include "TimeListLayer.h"

#define FONT_NAME                       "Thonburi"//"FZXS12--GB1-0.ttf"
//"FZXS12--GB1-0.ttf"
#define FONT_SIZE                       30
#define TAG_BG 31

#define TAG_ADD 20

TimeListLayer::TimeListLayer(void)
{
    timeLableArray = NULL;
}
TimeListLayer::~TimeListLayer(void)
{
    timeLableArray->release();
}

void TimeListLayer::addTimeItem(const char *szPicFileName, const char *szTimeFormat)
{
    return;
}

void TimeListLayer::onEnter()
{
    CCLayer::onEnter();
    timeLableArray = CCArray::create();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    std::string a = CCUserDefault::sharedUserDefault()->getStringForKey(CCString::createWithFormat("%dcontent", curLocation)->getCString(), "");
    
    CCLOG("timelist ENTER: %s", a.c_str());
    
    // if has something
    if ( strcmp(a.c_str(), "") != 0 )
    {
        CCLOG("has some... %d ", curLocation);
        std::string split = ",";
        CCArray* strs =  splitEx(a, split);
        
        // 1 png name, 2 future time, 3 inteval time
        for ( int i = 0; i < strs->count(); i+=3)
        {
            CCString* pngStr = (CCString*)(strs->objectAtIndex(i));
            long futureTime = atol( ((CCString*)(strs->objectAtIndex(i+1)))->getCString() );
            long inteval = atol( ((CCString*)(strs->objectAtIndex(i+2)))->getCString() );
            
            GameSprite* ebike = GameSprite::gameSpriteWithFileFutureTime(pngStr->getCString(),futureTime);
            
            struct tm *tm;
            long test1 = ebike->getFutureTime();
            tm = localtime(&test1);
            //CCLOG("start get futuretime%d : %d-%d-%d %d:%d:%d",i,tm->tm_year,tm->tm_mon,tm->tm_mday, tm->tm_hour ,tm->tm_min, tm->tm_sec);
            test1 = ebike->getCurrentTime();
            tm = localtime(&test1);
            //CCLOG("start get currenttime%d : %d-%d-%d %d:%d:%d",i,tm->tm_year,tm->tm_mon,tm->tm_mday, tm->tm_hour ,tm->tm_min, tm->tm_sec);
            long deltatime = ebike->getFutureTime() - ebike->getCurrentTime();
            long day = deltatime / 60 / 60 / 24;
            //CCLOG("delta day %ldd", day);
            //CCLOG("-----");
            ebike->setPosition(ccp(100+150*i/3, visibleSize.height/2+200));
            ebike->setInteval(inteval);
            ebike->itemTick();
            
            this->addChild(ebike);
            
            timeLableArray->addObject(ebike);
        }
        
        CCSprite* addSprite = CCSprite::create("add.png");
        addSprite->setPosition(ccp(100+150*(strs->count())/3, visibleSize.height/2+200));
        this->addChild(addSprite);
        addSprite->setTag(TAG_ADD);
        
    } else // if nothing
    {
        //CCLOG("xxxxxx NOTHING! %d ", curLocation);
        GameSprite* ebike = GameSprite::gameSpriteWithFile("ebike.png","5d");
        ebike->setPosition(ccp(visibleSize.width/2, visibleSize.height/2+200));
        this->addChild(ebike);
        
        GameSprite* ebike1 = GameSprite::gameSpriteWithFile("toothbrush.png","3d");
        ebike1->setPosition(ccp(visibleSize.width/2+150, visibleSize.height/2+200));
        this->addChild(ebike1);
        
        GameSprite* ebike2 = GameSprite::gameSpriteWithFile("laundry.png","3d");
        ebike2->setPosition(ccp(visibleSize.width/2+300, visibleSize.height/2+200));
        this->addChild(ebike2);
        
        GameSprite* ebike3 = GameSprite::gameSpriteWithFile("run.png","3d");
        ebike3->setPosition(ccp(visibleSize.width/2, visibleSize.height/2+200));
        this->addChild(ebike3);
        
        GameSprite* ebike4 = GameSprite::gameSpriteWithFile("test.png","1m5s");
        ebike4->setPosition(ccp(visibleSize.width/2+150, visibleSize.height/2+200));
        this->addChild(ebike4);
        
        
        timeLableArray->addObject(ebike);
        timeLableArray->addObject(ebike1);
        timeLableArray->addObject(ebike2);
        timeLableArray->addObject(ebike3);
        timeLableArray->addObject(ebike4);
        
        char ready_to_insert_content[200] = {0};
        
        for ( int i = 0; i < timeLableArray->count(); i++)
        {
            GameSprite* item = (GameSprite*)timeLableArray->objectAtIndex(i);
            if (i==0) {
                sprintf(ready_to_insert_content, "%s,%ld,%ld",
                        item->getPicFileName(), item->getFutureTime(), item->getInteval());
            } else {
                sprintf(ready_to_insert_content,
                        "%s,%s,%ld,%ld",
                        ready_to_insert_content,
                        item->getPicFileName(), item->getFutureTime(), item->getInteval());
            }
            item->setPosition(ccp(100+150*i, visibleSize.height/2+200));
        }
        
        CCSprite* addSprite = CCSprite::create("add.png");
        addSprite->setPosition(ccp(100+150*(timeLableArray->count()), visibleSize.height/2+200));
        this->addChild(addSprite);
        addSprite->setTag(TAG_ADD);
        
        CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%dcontent", curLocation)->getCString(), ready_to_insert_content);
    }
    
    timeLableArray->retain();
    float flipInterval = 1.0f;
    schedule(schedule_selector(TimeListLayer::UpdateAllLable), flipInterval);
    setTouchEnabled(true);
}

CCArray* TimeListLayer::getTimeLableArray()
{
    return timeLableArray;
}

bool TimeListLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
    
    int firstX=touchPoint.x;
    int firstY=touchPoint.y;
    
    touchPoint =  CCDirector::sharedDirector()->convertToUI(ccp(firstX, firstY));
    
    bool resetClock = false;
    
    if (getChildByTag(TAG_BG) != NULL) {
        removeChild(getChildByTag(TAG_BG));
        return true;
    }
    
    for ( int i = 0; i < timeLableArray->count(); i++)
    {
        GameSprite* item = (GameSprite*)timeLableArray->objectAtIndex(i);
        if (item->boundingBox().containsPoint(touchPoint))
        {
            item->resetClock();
            item->itemTick();
            resetClock = true;
            break;
        }
    }
    
    CCSprite* addSprite = (CCSprite*)getChildByTag(TAG_ADD);
    if (addSprite->boundingBox().containsPoint(touchPoint))
    {
        CCLOG("openIconBox");
        openIconBox();
    }
    
    if (resetClock) {
        char ready_to_insert_content[200] = {0};
        for ( int i = 0; i < timeLableArray->count(); i++)
        {
            GameSprite* item = (GameSprite*)timeLableArray->objectAtIndex(i);
            if (i==0) {
                sprintf(ready_to_insert_content, "%s,%ld,%ld",
                        item->getPicFileName(), item->getFutureTime(), item->getInteval());
            } else {
                sprintf(ready_to_insert_content,
                        "%s,%s,%ld,%ld",
                        ready_to_insert_content,
                        item->getPicFileName(), item->getFutureTime(), item->getInteval());
            }
        }
        CCLOG("- - - %s", ready_to_insert_content);
        if (strlen(ready_to_insert_content) > 0) {
            //CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%dcontent", curLocation)->getCString(), ready_to_insert_content);
            saveContentToMemory(ready_to_insert_content);
        }
    }
    
    return true;
}

void TimeListLayer::openIconBox()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    if (getChildByTag(TAG_BG) != NULL) {
        return;
    }

    CCSprite* bg = CCSprite::create("greyBG.jpg");
    CCSize bgSize = bg->getContentSize();
    bg->setScaleY(s.height/bgSize.height);
    bg->setScaleX(s.width/bgSize.width);
    bg->setPosition(ccp(s.width/2,s.height/2));
    //bg->setZOrder(-99);
    this->addChild(bg);
    bg->setTag(TAG_BG);
    bg->setOpacity(225);
    
    CCArray* iconNameArray = CCArray::create();
    iconNameArray->addObject(CCString::create("test.png"));
    iconNameArray->addObject(CCString::create("toothbrush.png"));
    iconNameArray->addObject(CCString::create("salon.png"));
    iconNameArray->addObject(CCString::create("ebike.png"));
    iconNameArray->addObject(CCString::create("laundry.png"));
    iconNameArray->addObject(CCString::create("run.png"));
    
    CCObject* itemObj;
    int i=0;
    CCARRAY_FOREACH(iconNameArray, itemObj)
    {
        CCSprite* myspt = CCSprite::create(((CCString*)itemObj)->getCString());
        myspt->setPosition(ccp(100+150*i, bg->getContentSize().height/2));
        bg->addChild(myspt);
        i++;
    }

}


bool TimeListLayer::saveContentToMemory(std::string ready_to_insert_content)
{
    CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%dcontent", curLocation)->getCString(), ready_to_insert_content);
}


void TimeListLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void TimeListLayer::setCurrentLocation(int value)
{
    curLocation = value;
}

void TimeListLayer::UpdateAllLable()
{
    CCObject *item = NULL;
    CCARRAY_FOREACH(timeLableArray, item)
    {
        GameSprite* eachItem = (GameSprite*)item;
        eachItem->itemTick();
    }
}

void TimeListLayer::onExit()
{
    CCLayer::onExit();
}


CCArray* TimeListLayer::splitEx(const std::string& src, std::string separate_character)
{
    CCArray* strs;
    strs = CCArray::create();
    
    int separate_characterLen = separate_character.size();//分割字符串的长度,这样就可以支持如“,,”多字符串的分隔符
    int lastPosition = 0,index = -1;
    while (-1 != (index = src.find(separate_character,lastPosition)))
    {
        strs->addObject(CCString::create(src.substr(lastPosition,index - lastPosition)));
        lastPosition = index + separate_characterLen;
    }
    std::string lastString = src.substr(lastPosition);//截取最后一个分隔符后的内容
    if (!lastString.empty())
    {
        strs->addObject(CCString::create(lastString));//如果最后一个分隔符后还有内容就入队
    }
    return strs;
}

