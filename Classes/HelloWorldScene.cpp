#include "HelloWorldScene.h"
#include "GBKParse.h"
USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum
{
    kTextFieldTTFDefaultTest = 0,
    kTextFieldTTFActionTest,
    kTextInputTestsCount,
};

#define FONT_NAME                       "FZXS12"
#define FONT_SIZE                       62

static int testIdx = -1;

static CCRect getRect(CCNode * pNode)
{
    CCRect rc;
    rc.origin = pNode->getPosition();
    rc.size = pNode->getContentSize();
    rc.origin.x -= rc.size.width / 2;
    rc.origin.y -= rc.size.height / 2;
    return rc;
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


TextFieldTTFActionTest* createTextInputTest(int nIndex, int location)
{
    TextFieldTTFActionTest* field = new TextFieldTTFActionTest();
    field->setLocation(location);
    return field;
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    pDict = CCDictionary::createWithContentsOfFile("strings1.plist");

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...
    
    // 初始化进入软件时候的节点位置
    location = 0;
    
    // 初始化用户输入框
    pTestLayer = createTextInputTest(testIdx, location);
    pTestLayer->autorelease();
    
    this->addKeyboardNotificationLayer(pTestLayer);
    
    // 初始化翻页间隔时间
    flipInterval = 3.0f;
    
    setTouchEnabled(true);
    
    schedule(schedule_selector(HelloWorld::Flip), flipInterval); //每隔flipInterval时间执行一次，省略参数则表示每帧都要执行
    
    return true;
}

void HelloWorld::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOG("touch bagan");
    CCPoint touchPoint = pTouch->getLocation();
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
    firstX=touchPoint.x;
    firstY=touchPoint.y;
    
    return true;
}


void HelloWorld::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    return;
}

void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    CCPoint touchPoint;
    touchPoint = pTouch->getLocation();
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
    endX=firstX - touchPoint.x;
    endY=firstY - touchPoint.y;
    
    if(abs(endX) < abs(endY))
    {
        if(abs(endY)>15)
        {
            if(endY+5>0)
            {
                doDown();
            }
            else
            {
                doUp();
            }
            
            finishTransAction(NULL);
        }
    }
    
    this->unschedule(schedule_selector(HelloWorld::Flip));
}

void HelloWorld::Flip(float dt)
{
    CCLOG("baibai");
    doDown();
    
    // nothing down there
    if(finishTransAction(NULL)==false)
    {
        location = 0;
        pTestLayer->setLocation(location);
        finishTransAction(NULL);
    }
}

CCString* HelloWorld::getStringFromSavedLocation(int loc)
{
    std::string a = CCUserDefault::sharedUserDefault()->getStringForKey(CCString::createWithFormat("%d", location)->getCString(), "");
    CCString* cca = CCString::create(a);
    return cca;
}

bool HelloWorld::finishTransAction(CCNode* pSender)
{
    
    CCLOG("loc:%d",location);
    
    std::string a = CCUserDefault::sharedUserDefault()->getStringForKey(CCString::createWithFormat("%d", location)->getCString(), "");
    CCString* cca = CCString::create(a);
    pTestLayer->m_pTextField->setString(cca->getCString());
    
    CCLOG("loc:%s",cca->getCString());
    
    pTestLayer->setVisible(true);
    
    hideProgressBar();
    
    if(cca==NULL || strcmp(cca->getCString(), "")==0)
    {
        unscheduleUpdate();
        if(progress1!=NULL)
        {
            progress1->setVisible(false);
            progressbgSprite->setVisible(false);
            numsTTF->setVisible(false);
            numsTTF1->setVisible(false);
        }
        return false;
    } else if(strstr(cca->getCString(), "")!=NULL) //(c) c means countdown
    {
        std::string anticipateTimeSec = a.substr(3,-1);
        CCString* ccb = CCString::create(anticipateTimeSec);
        CCLOG("-->%s", ccb->getCString());
        // OK, we got the anticipated seconds
        std::string split = ",";
        CCArray* strs = splitEx(anticipateTimeSec, split);
        
        if(strs->count()==2)
        {
            CCLOG("--1>%s", ((CCString*)strs->objectAtIndex(0))->getCString());
            CCLOG("--2>%s", ((CCString*)strs->objectAtIndex(1))->getCString());
            
            createProgressBar();
        }
        return true;
    } else {
        unscheduleUpdate();
        if(progress1!=NULL)
        {
            progress1->setVisible(false);
            progressbgSprite->setVisible(false);
            numsTTF->setVisible(false);
            numsTTF1->setVisible(false);
        }
    }
    return true;
}


void HelloWorld::hideProgressBar()
{
    unscheduleUpdate();
    if(progress1!=NULL)
    {
        progress1->setVisible(false);
        progressbgSprite->setVisible(false);
        numsTTF->setVisible(false);
        numsTTF1->setVisible(false);
    }

}

void HelloWorld::createProgressBar()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    //创建一个进度条精灵，这个是2.0以后api变了
    progressbgSprite=CCSprite::create("time_slot_hd.png");
    
    int positionX = visibleSize.width/2;
    int positionY = visibleSize.height/4-progressbgSprite->getContentSize().height/2;
    
    progressbgSprite->setPosition(ccp(positionX,positionY
                                      ));
    
    this->addChild(progressbgSprite, 1);
    
    
    CCSprite *progressSprite=CCSprite::create("time_bars_hd.png");
    
    progress1=CCProgressTimer::create(progressSprite);
    
    progress1->setType(kCCProgressTimerTypeBar);
    
    progress1->setPosition(ccp(positionX, positionY));
    
    //进度动画运动方向，可以多试几个值，看看效果
    progress1->setMidpoint(ccp(0, 0));
    
    //进度条宽高变化
    progress1->setBarChangeRate(ccp(1, 0));
    
    progress1->setPercentage(0);
    
    this->addChild(progress1, 1);
    
    numsTTF=CCLabelTTF::create("0", "Thonburi", 18);
    
    
    numsTTF->setPosition(ccp(positionX, positionY));
    
    
    this->addChild(numsTTF, 1);
    
    numsTTF1=CCLabelTTF::create("0", "Thonburi", 18);
    
    numsTTF1->setPosition(ccp(positionX, positionY+50));
    
    
    numsTTF1->setString("今年过去了");
    this->addChild(numsTTF1, 1);
    
    
    struct tm *tm;
    time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    time(&timep);
#else
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    timep = now.tv_sec;
#endif
    
    
    tm = localtime(&timep);
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    int hour=tm->tm_hour;
    int min=tm->tm_min;
    int second=tm->tm_sec;
    
    secPassedSinceMidNight = hour*60.0*60+min*60.0+second;
    
    percentTodayOfYear = (tm->tm_yday*100.0-100.0/(24*60*60)*(24.0*60.0*60.0-secPassedSinceMidNight))/365.0;
    CCString *str1 = CCString::createWithFormat("%.10f%%",percentTodayOfYear);  
    
    numsTTF->setString(UTEXT(str1->getCString()));
    progress1->setPercentage(percentTodayOfYear);
    
    this->unscheduleUpdate();
    this->scheduleUpdate();

}

void HelloWorld::update(float t)
{
    struct tm *tm;
    time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    time(&timep);
#else
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    timep = now.tv_sec;
#endif
    
    tm = localtime(&timep);
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    int hour=tm->tm_hour;
    int min=tm->tm_min;
    int second=tm->tm_sec;
    
    secPassedSinceMidNight += t;
    percentTodayOfYear = (tm->tm_yday*100.0+(100.0/(24*60*60)*(secPassedSinceMidNight)))/365.0;
    CCString *str1 = CCString::createWithFormat("%.10f%%",percentTodayOfYear);
    
    numsTTF->setString(UTEXT(str1->getCString()));
    progress1->setPercentage(percentTodayOfYear);
}


CCArray* HelloWorld::splitEx(const std::string& src, std::string separate_character)
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

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::addKeyboardNotificationLayer(KeyboardNotificationLayer * pLayer)
{
    m_pNotificationLayer = pLayer;
    addChild(pLayer);
}


//////////////////////////////////////////////////////////////////////////
// implement KeyboardNotificationLayer
//////////////////////////////////////////////////////////////////////////

KeyboardNotificationLayer::KeyboardNotificationLayer()
: m_pTrackNode(0)
{
    setTouchEnabled(true);
}

void KeyboardNotificationLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void KeyboardNotificationLayer::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    //CCLOG("TextInputTest:keyboardWillShowAt(origin:%f,%f, size:%f,%f)",
    //	info.end.origin.x, info.end.origin.y, info.end.size.width, info.end.size.height);
    
    if (! m_pTrackNode)
    {
        return;
    }
    
    CCRect rectTracked = getRect(m_pTrackNode);
    //CCLOG("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
    //	rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);
    
    // if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
    if (! rectTracked.intersectsRect(info.end))
    {
        return;
    }
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
    CCLOG("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);
    
    // move all the children node of KeyboardNotificationLayer
    CCArray * children = getChildren();
    CCNode * node = 0;
    int count = children->count();
    CCPoint pos;
    for (int i = 0; i < count; ++i)
    {
        node = (CCNode*)children->objectAtIndex(i);
        pos = node->getPosition();
        pos.y += adjustVert;
        node->setPosition(pos);
    }
}

// CCLayer function

bool KeyboardNotificationLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    //CCLOG("++++++++KeyboardNotificationLayer::ccTouchBegan+++++++++++++");
    m_beginPos = pTouch->getLocation();
    return true;
}

void KeyboardNotificationLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (! m_pTrackNode)
    {
        return;
    }
    
    CCPoint endPos = pTouch->getLocation();
    
    float delta = 5.0f;
    if (::abs(endPos.x - m_beginPos.x) > delta
        || ::abs(endPos.y - m_beginPos.y) > delta)
    {
        // not click
        m_beginPos.x = m_beginPos.y = -1;
        return;
    }
    
    // decide the trackNode is clicked.
    CCRect rect;
    CCPoint point = convertTouchToNodeSpaceAR(pTouch);
    //CCLOG("KeyboardNotificationLayer:clickedAt(%f,%f)", point.x, point.y);
    
    rect = getRect(m_pTrackNode);
    //CCLOG("KeyboardNotificationLayer:TrackNode at(origin:%f,%f, size:%f,%f)",
    //	rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    
    this->onClickTrackNode(rect.containsPoint(point));
    //CCLOG("----------------------------------");
}


//////////////////////////////////////////////////////////////////////////
// implement TextFieldTTFActionTest
//////////////////////////////////////////////////////////////////////////

std::string TextFieldTTFActionTest::subtitle()
{
    return "CCTextFieldTTF with action and char limit test";
}

void TextFieldTTFActionTest::onClickTrackNode(bool bClicked)
{
    CCTextFieldTTF * pTextField = (CCTextFieldTTF*)m_pTrackNode;
    if (bClicked)
    {
        // TextFieldTTFTest be clicked
        CCLOG("TextFieldTTFActionTest:CCTextFieldTTF attachWithIME");
        pTextField->attachWithIME();
    }
    else
    {
        // TextFieldTTFTest not be clicked
        CCLOG("TextFieldTTFActionTest:CCTextFieldTTF detachWithIME");
        pTextField->detachWithIME();
    }
}

void TextFieldTTFActionTest::onEnter()
{
    KeyboardNotificationLayer::onEnter();
    
    m_nCharLimit = 18;
    
    m_pTextFieldAction = CCRepeatForever::create(
                                                 CCSequence::create(
                                                                    CCFadeOut::create(0.25),
                                                                    CCFadeIn::create(0.25),
                                                                    0
                                                                    ));
    m_pTextFieldAction->retain();
    m_bAction = false;
    
    // add CCTextFieldTTF
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    m_pTextField = CCTextFieldTTF::textFieldWithPlaceHolder("<click here for input>",
                                                            FONT_NAME,
                                                            FONT_SIZE);
    addChild(m_pTextField);
    
    float hscale = s.height/4;
    
    float wscale = s.width/m_pTextField->getContentSize().width;
    
    m_pTextField->setScale(hscale>wscale?wscale:hscale);
    std::string a = CCUserDefault::sharedUserDefault()->getStringForKey(CCString::createWithFormat("%d", this->curLocation)->getCString(), "");
    CCString* cca = CCString::create(a);
    m_pTextField->setString(cca->getCString());
    
    m_pTextField->setDelegate(this);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // on android, CCTextFieldTTF cannot auto adjust its position when soft-keyboard pop up
    // so we had to set a higher position
    m_pTextField->setPosition(ccp(s.width / 2, s.height /2 + 50));
#else
    m_pTextField->setPosition(ccp(s.width / 2, s.height / 2));
#endif
    m_pTrackNode = m_pTextField;
}

void TextFieldTTFActionTest::onExit()
{
    KeyboardNotificationLayer::onExit();
    m_pTextFieldAction->release();
}

// CCTextFieldDelegate protocol
bool TextFieldTTFActionTest::onTextFieldAttachWithIME(CCTextFieldTTF * pSender)
{
    if (! m_bAction)
    {
        m_pTextField->runAction(m_pTextFieldAction);
        m_bAction = true;
    }
    return false;
}

bool TextFieldTTFActionTest::onTextFieldDetachWithIME(CCTextFieldTTF * pSender)
{
    if (m_bAction)
    {
        m_pTextField->stopAction(m_pTextFieldAction);
        m_pTextField->setOpacity(255);
        m_bAction = false;
        
        if(strcmp(pSender->getString(),"")!=0)
        {
            CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%d", this->curLocation)->getCString(), pSender->getString());
            
        }
    }
    return false;
}

bool TextFieldTTFActionTest::onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen)
{
    // if insert enter, treat as default to detach with ime
    if ('\n' == *text)
    {
        return false;
    }
    
    // if the textfield's char count more than m_nCharLimit, doesn't insert text anymore.
    if (pSender->getCharCount() % m_nCharLimit > (m_nCharLimit-3))
    {
        char toshow[100];
        strcpy(toshow,"");
        strcat(toshow,m_pTextField->getString());
        strcat(toshow,"\n");
        this->m_pTextField->setString(toshow);
    }
    
    // create a insert text sprite and do some action
    CCLabelTTF * label = CCLabelTTF::create(text, FONT_NAME, FONT_SIZE);
    this->addChild(label);
    ccColor3B color = { 226, 121, 7};
    label->setColor(color);
    
    // move the sprite from top to position
    CCPoint endPos = pSender->getPosition();
    if (pSender->getCharCount())
    {
        endPos.x += pSender->getContentSize().width / 2;
    }
    CCSize  inputTextSize = label->getContentSize();
    CCPoint beginPos(endPos.x, CCDirector::sharedDirector()->getWinSize().height - inputTextSize.height * 2);
    
    float duration = 0.5;
    label->setPosition(beginPos);
    label->setScale(8);
    
    CCAction * seq = CCSequence::create(
                                        CCSpawn::create(
                                                        CCMoveTo::create(duration, endPos),
                                                        CCScaleTo::create(duration, 1),
                                                        CCFadeOut::create(duration),
                                                        0),
                                        CCCallFuncN::create(this, callfuncN_selector(TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction)),
                                        0);
    label->runAction(seq);
    return false;
}

bool TextFieldTTFActionTest::onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen)
{
    // create a delete text sprite and do some action
    CCLabelTTF * label = CCLabelTTF::create(delText, FONT_NAME, FONT_SIZE);
    this->addChild(label);
    
    // move the sprite to fly out
    CCPoint beginPos = pSender->getPosition();
    CCSize textfieldSize = pSender->getContentSize();
    CCSize labelSize = label->getContentSize();
    beginPos.x += (textfieldSize.width - labelSize.width) / 2.0f;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint endPos(- winSize.width / 4.0f, winSize.height * (0.5 + (float)rand() / (2.0f * RAND_MAX)));
    
    float duration = 1;
    float rotateDuration = 0.2f;
    int repeatTime = 5; 
    label->setPosition(beginPos);
    
    CCAction * seq = CCSequence::create(
                                        CCSpawn::create(
                                                        CCMoveTo::create(duration, endPos),
                                                        CCRepeat::create(
                                                                         CCRotateBy::create(rotateDuration, (rand()%2) ? 360 : -360),
                                                                         repeatTime),
                                                        CCFadeOut::create(duration),
                                                        0),
                                        CCCallFuncN::create(this, callfuncN_selector(TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction)),
                                        0);
    label->runAction(seq);
    return false;
}

bool TextFieldTTFActionTest::onDraw(CCTextFieldTTF * pSender)
{
    return false;
}

void TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction(CCNode * pNode)
{
    this->removeChild(pNode, true);
}

bool HelloWorld::doUp()
{
    CCLOG("doUp");
    location += 1000;
    pTestLayer->setLocation(location);
    

}

bool HelloWorld::doDown()
{
    CCLOG("doDown");
    location -= 1000;
    pTestLayer->setLocation(location);

}

void TextFieldTTFActionTest::setLocation( int location )
{
    this->curLocation = location;
}
