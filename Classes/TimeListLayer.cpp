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
    strcpy(clickItemName, "aaa--");
}
TimeListLayer::~TimeListLayer(void)
{
    timeLableArray->release();
}


void TimeListLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

void TimeListLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void TimeListLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
    CCLog("%p", clickItem);
    CCLog("%s", clickItem->getPicFileName());
    //strcpy(clickItemName,"aaa--");
    //strcat(clickItemName,text.c_str());
    CCLog("%s", clickItemName);
}

void TimeListLayer::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p was returned !",editBox);
    
    if (m_pEditTimeFormatString == editBox) {
        long futureTime = clickItem->getFutureTimeFromUserFormatTimeString(CCString::create(editBox->getText()));
        long currentTime = clickItem->getCurrentTime();
        long inteval = futureTime - currentTime;
        
        if (clickItem==NULL || editBox->getText() == NULL || strcmp(editBox->getText(), "") == 0 ) {
            return;
        }
        
        //appendNewItemToSavedContent(clickItem->getPicFileName(),"");
        std::string a = CCUserDefault::sharedUserDefault()->getStringForKey(CCString::createWithFormat("%dcontent", curLocation)->getCString(), "");
        
        clickItem->setStringLable(editBox->getText());
        
        CCString* mynewStringToSave = NULL;
        if ( strcmp(a.c_str(), "") != 0 )
        {
            // add a commet
            mynewStringToSave = CCString::createWithFormat("%s,%s,%ld,%ld ", a.c_str(), clickItem->getPicFileName(), futureTime, inteval);
        } else {
            mynewStringToSave = CCString::createWithFormat("%s,%ld,%ld ", clickItem->getPicFileName(), futureTime, inteval);
        }
        CCLog("%s", mynewStringToSave->getCString());
        
        CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%dcontent", curLocation)->getCString(), mynewStringToSave->getCString());
    }

    if (m_pEditYourOwnName == editBox) {
        clickItem->setString(editBox->getText());
        clickItem->setPicFileName(editBox->getText());
    }
}

void TimeListLayer::appendNewItemToSavedContent(const char* name, const char* mytime)
{
    
    std::string a = CCUserDefault::sharedUserDefault()->getStringForKey(CCString::createWithFormat("%dcontent", curLocation)->getCString(), "");
    
    char ready_to_insert_content[100] ={0};
    
    if ( strcmp(a.c_str(), "") != 0 )
    {
        // add a commet
        strcpy(ready_to_insert_content, a.c_str());
        strcat(ready_to_insert_content, ",");
    }
    
    GameSprite* tempitem = GameSprite::gameSpriteWithFile(name,"2d");
    
    sprintf(ready_to_insert_content,
            "%s%s,%ld,%ld",
            ready_to_insert_content,
            name, tempitem->getFutureTime(), tempitem->getInteval());
    
    CCLog("---res:%s",ready_to_insert_content);

}

void TimeListLayer::addTimeItem(const char *szPicFileName, const char *szTimeFormat)
{
    return;
}

void TimeListLayer::onEnter()
{
    CCLayer::onEnter();
    m_pEditTimeFormatString = NULL;
    m_pEditYourOwnName = NULL;
    timeLableArray = CCArray::create();
    
    clickItem = NULL;
    
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
            test1 = ebike->getCurrentTime();
            tm = localtime(&test1);
            //long deltatime = ebike->getFutureTime() - ebike->getCurrentTime();
            ebike->setPosition(ccp(100+(150*i/3)%(int)(visibleSize.width-300), visibleSize.height-100-150*((150*i/3)/(int)(visibleSize.width-300))));
            ebike->setInteval(inteval);
            ebike->itemTick();
            
            this->addChild(ebike);
            
            timeLableArray->addObject(ebike);
        }
        
        CCSprite* addSprite = CCSprite::create("add.png");
        int i = strs->count();
        addSprite->setPosition(ccp(100+(150*i/3)%(int)(visibleSize.width-300), visibleSize.height-100-150*((150*i/3)/(int)(visibleSize.width-300))));
        this->addChild(addSprite);
        addSprite->setTag(TAG_ADD);
        
    } else // if nothing
    {
        //CCLOG("xxxxxx NOTHING! %d ", curLocation);
        
        /*
        
        GameSprite* ebike = GameSprite::gameSpriteWithFile("ebikee","5d");
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
        timeLableArray->addObject(ebike4);*/
        
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
        
        CCLog("--%s",ready_to_insert_content);
        
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
    
    // 如果有二级对话框
    if (getChildByTag(TAG_BG) != NULL) {
        
        CCSprite* mybg = (CCSprite*)getChildByTag(TAG_BG);
        
        // get bg rate
        CCSize s = CCDirector::sharedDirector()->getWinSize();
        CCSize bgSize = mybg->getContentSize();
        
        float rateX = s.width/bgSize.width;
        float rateY = s.height/bgSize.height;
        
        bool tapOnItem = false;
        
        // 看看有没有点到什么
        for (int i=0; i<500; i++) {
            int mytag = i+1000;
            GameSprite* myItem = (GameSprite*)mybg->getChildByTag(mytag);
            if (myItem!=NULL) {
                // 点到图标
                if (myItem->isVisible()
                    && myItem->boundingBox().containsPoint(ccp(touchPoint.x/rateX, touchPoint.y/rateY)))
                {
                    tapOnItem = true;
                    clickItem = myItem;
                    
                    bool yourOwnWord = false;
                    
                    if (strcmp(clickItem->getPicFileName(), "Ur own word") == 0) {                        yourOwnWord = true;
                    }
                    
                    if (!m_pEditTimeFormatString->isVisible()) {
                        m_pEditTimeFormatString->setVisible(true);
                    }
                    
                    if (yourOwnWord && !m_pEditYourOwnName->isVisible()) {
                        m_pEditYourOwnName->setVisible(true);
                    }
                    
                    // invisible all other items
                    for (int j=0; j<500; j++) {
                        int mytag2 = j+1000;
                        GameSprite* myItem2 = (GameSprite*)mybg->getChildByTag(mytag2);
                        if (myItem2!=NULL) {
                            myItem2->setVisible(false);
                            
                        } else {
                            break;
                        }
                    }
                    myItem->setVisible(true);
                    break;
                }
            } else {
                break;
            }
        }
        
        if (m_pEditTimeFormatString!=NULL && m_pEditTimeFormatString->isVisible() && m_pEditTimeFormatString->boundingBox().containsPoint(ccp(touchPoint.x, touchPoint.y))) {
            tapOnItem = true;
        }
        
        if (m_pEditYourOwnName!=NULL && m_pEditYourOwnName->isVisible() && m_pEditYourOwnName->boundingBox().containsPoint(ccp(touchPoint.x, touchPoint.y))) {
            tapOnItem = true;
        }
        
        // 什么都没有点到 关闭
        if (!tapOnItem) {
            removeChild(getChildByTag(TAG_BG));
            m_pEditTimeFormatString->setVisible(false);
            m_pEditYourOwnName->setVisible(false);
            
            clickItem = NULL;
        }
        
        return false;
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
    
    iconNameArray->addObject(CCString::create("Ur own word"));
    
    CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
    CCSize editBoxSize = CCSizeMake(s.width - 100, 60);
    
    // top
    if (m_pEditTimeFormatString == NULL) {
        
        m_pEditTimeFormatString = cocos2d::extension::CCEditBox::create(editBoxSize, cocos2d::extension::CCScale9Sprite::create("orange_edit.png"));
        m_pEditTimeFormatString->setPosition(ccp(visibleOrigin.x+editBoxSize.width/2, visibleOrigin.y+editBoxSize.height*3/4));
        m_pEditTimeFormatString->setFontSize(25);
        m_pEditTimeFormatString->setFontColor(ccRED);
        m_pEditTimeFormatString->setPlaceHolder("Input how long to expire.(Format: ?d?h)");
        m_pEditTimeFormatString->setPlaceholderFontColor(ccWHITE);
        m_pEditTimeFormatString->setMaxLength(10);
        m_pEditTimeFormatString->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);
        m_pEditTimeFormatString->setDelegate(this);
        this->addChild(m_pEditTimeFormatString);
        m_pEditTimeFormatString->setVisible(false);
    } else {
        //m_pEditTimeFormatString->setVisible(true);
        m_pEditTimeFormatString->setZOrder(100);
    }
    
    
    
    
    if (m_pEditYourOwnName == NULL) {
        
        m_pEditYourOwnName = cocos2d::extension::CCEditBox::create(editBoxSize, cocos2d::extension::CCScale9Sprite::create("orange_edit.png"));
        m_pEditYourOwnName->setPosition(ccp(visibleOrigin.x+editBoxSize.width/2, visibleOrigin.y+editBoxSize.height*3/4*4));
        m_pEditYourOwnName->setFontSize(25);
        m_pEditYourOwnName->setFontColor(ccRED);
        m_pEditYourOwnName->setPlaceHolder("Type your own word instead of icon.");
        m_pEditYourOwnName->setPlaceholderFontColor(ccWHITE);
        m_pEditYourOwnName->setMaxLength(10);
        m_pEditYourOwnName->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);
        m_pEditYourOwnName->setDelegate(this);
        this->addChild(m_pEditYourOwnName);
        m_pEditYourOwnName->setVisible(false);
    } else {
        //m_pEditTimeFormatString->setVisible(true);
        m_pEditYourOwnName->setZOrder(100);
    }
    
    CCObject* itemObj;
    int i = 0;
    CCARRAY_FOREACH(iconNameArray, itemObj)
    {
        GameSprite* myspt = GameSprite::gameSpriteWithFileFutureTime(((CCString*)itemObj)->getCString(),0);
        
        myspt->setPosition(ccp(100+150*i, bg->getContentSize().height/2));
        bg->addChild(myspt);
        myspt->setTag(1000+i);
        
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

