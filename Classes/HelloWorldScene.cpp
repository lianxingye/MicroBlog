#include "HelloWorldScene.h"
#include "GBKParse.h"
USING_NS_CC;

static int html_static_count = 0;
static CCDictionary* html_static_dict = NULL;

#define TAG_PACE_LABEL 250

#define TAG_DAILY 20
#define TAG_COUNTER_LABEL 21
#define TAG_ENVELOP_PNG 22

//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

enum
{
    kTextFieldTTFDefaultTest = 0,
    kTextFieldTTFActionTest,
    kTextInputTestsCount,
};

#define DEF_FONT_NAME                   "Thonburi"
#define FONT_NAME                       "FZXS12--GB1-0.ttf"
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
    
    if (html_static_dict==NULL) {
        html_static_dict = CCDictionary::create();
        html_static_dict->retain();
    }

    
    // 'scene' is an autorelease object
    //CCScene *scene = CCScene::create();
    
    
    CCScene* scene = HelloWorld::transScene(0,1,0);

    // 'layer' is an autorelease object
    //HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    //scene->addChild(layer);

    // return the scene
    return scene;
}

bool HelloWorld::checkIfPositionEmpty(CCString* cca)
{
    if(cca==NULL || strcmp(cca->getCString(), "")==0)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

// This will goto the buffer to get the text at the beginning
CCScene* HelloWorld::transScene(int position = 0, int mannualTrans = 0, int type = 0)
{
    DateTimeManage::getCurrentTime();

    time_t mytime;
    mytime = 0;
    DateTimeManage::howManySecSince(mytime);

    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // because here need to get static position, so you will never get private location here, get outside
    layer->refreshFrameByLocation(position);

    // check if need progressbar
    CCString* cca = layer->getStringFromSavedLocation(position);
    
    CCLOG("in pos:%d, str=%s", position, cca->getCString());
    
    // if it is auto trans, do not show empty cells
    if(mannualTrans==0)
    {
        //if(checkIfPositionEmpty(cca))
        if(cca==NULL || strcmp(cca->getCString(), "")==0)
        {
            //  回到这一列的第一个
            CCLOG("redi from: %d", position);
            
            position = position % 1000;
            
            CCLOG("redi to: %d", position);
            cca = layer->getStringFromSavedLocation(position);
            
            std::string a = CCUserDefault::sharedUserDefault()->getStringForKey(CCString::createWithFormat("%d", position)->getCString(), "");
            
            //strange use getStringFromSavedLoc can not get the cca string
            // so use the direct way here
            cca = CCString::create(a);
            
            CCLOG("in pos%d, str=%s",position,CCString::create(a)->getCString());
            layer->refreshFrameByLocation(position);
        }
    }
    layer->showPostionOnFrame();
    //layer->show4Grid(1111);
    layer->showFirstFrameIcon();
    
    layer->checkIfCounterNeeded(cca);
    layer->checkIfProgressBarNeeded(cca);
    layer->checkIfHTMLMessengerNeeded(cca);
    layer->checkIfDailyNeeded(cca);
    layer->checkIfPaceNeeded(cca);
    layer->checkIfHalfCompNeeded(cca);
    layer->checkIfSanAnimNeeded(cca);
    
    layer->checkIfBallsNeeded(cca);
    layer->checkifEnvelopeNeeded(cca);
    
    
    
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
    
    typea = typeb = typec = typed = 1;
    
    touchForPaceEnabled = false;
    allDistanceByCm=0;
    paceByCm = 75;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    pDict = CCDictionary::createWithContentsOfFile("strings1.plist");
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    
    // add a "next frame" icon to animatedly jump to next frame
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "stop-button-hi.png",
                                        "stop-button-hi1.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));

	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // add a "next frame" icon to animatedly jump to next frame
    CCMenuItemImage *pCloseItemNextFrame = CCMenuItemImage::create(
                                        "pause_dark.png",
                                        "pause_light.png",
                                        this,
                                        menu_selector(HelloWorld::menuNextFrameCallback));

    pCloseItemNextFrame->setPosition(ccp(origin.x + visibleSize.width - pCloseItemNextFrame->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2 + pCloseItemNextFrame->getContentSize().height));
    
    pDelItemNextFrame = CCMenuItemImage::create(
                                         "Delete-Button.png",
                                         "Delete-Button.png",
                                         this,
                                         menu_selector(HelloWorld::menuDelFrameCallback));
    
    pDelItemNextFrame->setPosition(ccp(origin.x + visibleSize.width - pCloseItemNextFrame->getContentSize().width/2  ,
                                         pCloseItemNextFrame->getPositionY() + pDelItemNextFrame->getContentSize().height));
    
/////////
    pCloseItemAddButton = CCMenuItemImage::create(
                                                                   "addbutton.png",
                                                                   "addbutton.png",
                                                                   this,
                                                                   menu_selector(HelloWorld::menuAddFrameCallback));
    
    pCloseItemAddButton->setPosition(ccp(origin.x + visibleSize.width - pCloseItemAddButton->getContentSize().width/2 ,
                                         origin.y + pDelItemNextFrame->getPositionY() + pCloseItemAddButton->getContentSize().height));
    
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pCloseItemNextFrame, pDelItemNextFrame,pCloseItemAddButton, NULL);
    pMenu->setPosition(CCPointZero);
    
    this->addChild(pMenu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    // 初始化进入软件时候的节点位置
    location = 0;

    // 初始化用户输入框
    
    pTestLayer = createTextInputTest(testIdx, location);
    pTestLayer->autorelease();
    
    
    // move the creation of text file middle ahead, because it need some position change in duration
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    pTestLayer->m_pTextField = CCTextFieldTTF::textFieldWithPlaceHolder("<click here for input>",
                                                            FONT_NAME,
                                                            FONT_SIZE);
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // on android, CCTextFieldTTF cannot auto adjust its position when soft-keyboard pop up
    // so we had to set a higher position
    pTestLayer->m_pTextField->setPosition(ccp(s.width / 2, s.height /2 + 50));
#else
    pTestLayer->m_pTextField->setPosition(ccp(s.width / 2, s.height / 2));
#endif
    
    //showPostionOnFrame();

    this->addKeyboardNotificationLayer(pTestLayer);
    
    // 初始化翻页间隔时间
    flipInterval = 3.0f;

    setTouchEnabled(true);

    schedule(schedule_selector(HelloWorld::Flip), flipInterval); //每隔flipInterval时间执行一次，省略参数则表示每帧都要执行
    
    //if(location/1000==0)
    //{
    //    showFirstFrameIcon();
    //}
    
    //红色圆形进度条
    CCSprite* roundSprite = CCSprite::create("round_progress.png");
    CCProgressTimer *pProgressTimer = CCProgressTimer::create(roundSprite);
    CCSize szWin = CCDirector::sharedDirector()->getVisibleSize();
    pProgressTimer->setPosition(CCPointMake(szWin.width-roundSprite->getContentSize().width/2
                                            ,szWin.height-roundSprite->getContentSize().height/2));
    pProgressTimer->setPercentage(0);//显示原形的百分比
    this->addChild(pProgressTimer,0,100);
    this->schedule(schedule_selector(HelloWorld::UpdateProgress));//更加实际情况来更新进度.这里用定时器以便演示
    
    return true;
}

void HelloWorld::onHttpRequestCompleted(cocos2d::CCNode *sender ,void *data)
{
    //AllocConsole();
    //freopen("CONIN$", "r", stdin);
    //freopen("CONOUT$", "w", stdout);
    //freopen("CONOUT$", "w", stderr);
    CCLOG("OK, we have HTML response");
    
    cocos2d::extension::CCHttpResponse *response = (cocos2d::extension::CCHttpResponse*)data;
    if (!response)
    {
        return;
    }
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString ,"Http status code:%d ,tag = %s" ,statusCode ,response->getHttpRequest()->getTag());
    CCLog("response code:%d" ,statusCode);
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer:%s" ,response->getErrorBuffer());
        
        //没有网
        
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        int positionX = visibleSize.width/2;
        int positionY = visibleSize.height*3/4;
        
        CCLabelTTF* pmlabel1 = CCLabelTTF::create("Please check WiFi", FONT_NAME, 40);
        pmlabel1->setPosition(ccp(positionX, positionY+20));
        this->addChild(pmlabel1);
        return;
        
    }
    std::vector<char> *buffer = response->getResponseData();
    
    std::string str_buf;
    
    CCLOG("Http response,dump data:");
    std::string result = "";
    for (unsigned int i = 0; i < buffer->size(); i ++)
    {
        str_buf.push_back((*buffer)[i]);
        //CCLOG("%c" ,(*buffer)[i]);
    }
    CCString* mybuf = CCString::create(str_buf);
    //CCLOG("====here we go====%s", mybuf->getCString());
    
    // the key1 is used for position the only place in all the html garbage
    CCString* key1=ccs("class=\"op_pm25_graexp");
    CCString* key2=ccs(">");
    CCString* key3=ccs("</span>");
    
    CCString* result_ccstring = ThreeWordFindFromHTML(mybuf, key1, key2 ,key3);
    
    
    CCLOG("======set pa1=====%s", result_ccstring->getCString());
    result_ccstring->retain();
    html_static_dict->setObject(result_ccstring, "pm1");
    
    // the key1 is used for position the only place in all the html garbage
    key1=ccs("class=\"op_pm25_date");
    key2=ccs(">");
    key3=ccs("</span>");
    
    CCString* result_ccstring1 = ThreeWordFindFromHTML(mybuf, key1, key2 ,key3);
    CCLOG("======set pa2====%s %d", result_ccstring1->getCString(), html_static_count++);
    result_ccstring1->retain();
    html_static_dict->setObject(result_ccstring1, "pm2");
    
    
    createHTMLBar();
}

CCString* HelloWorld::ThreeWordFindFromHTML(CCString* source, CCString* key1, CCString* key2, CCString* key3)
{
    char* a, *b;
    a = strstr(source->getCString(),key1->getCString());
    //CCLOG("====here we go2====%s", a);
    
    // the key2 is used for precice word before the target
    
    if ( a==NULL )
    {
// 没有网
        return NULL;
    }
    
    a = strstr(a,key2->getCString());
    
    a = a + key2->length();
    
    //CCLOG("====here we go3====%s", a);
    
    b = strstr(a,key3->getCString());
    
    size_t len = b-a;
    
    //char result_char_array[100];
    
    char *result_char_array = (char*)malloc(sizeof(char)*(len+1));
    
    strncpy(result_char_array, a, len);
    result_char_array[len] = '\0';
    
    CCString* result_ccstring = CCString::createWithFormat("%s", result_char_array);
    
    //CCLOG("====here we go4====%s", result_ccstring->getCString());
    
    free(result_char_array);
    result_char_array = NULL;
    
    return result_ccstring;
}

void HelloWorld::UpdateProgress(float Dt)
{
    CCProgressTimer * pProgressTimer = (CCProgressTimer *)this->getChildByTag(100);

    pProgressTimer->setPercentage(pProgressTimer->getPercentage() + Dt * 100 / 3);//更新进度
    if (pProgressTimer->getPercentage()==100)
    {
        this->unschedule(schedule_selector(HelloWorld::UpdateProgress));//取消定时器
    }
}

void HelloWorld::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void HelloWorld::touchForPace()
{
    if (touchForPaceEnabled == true) {
        CCLOG("touch pace yes");
        allDistanceByCm += paceByCm;
        CCLOG("%d", allDistanceByCm);
        refreshPaceLabel();
    } else
    {
        ;
    }
}

bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
    firstX=touchPoint.x;
    firstY=touchPoint.y;
    
    touchForPace();

    return true;
}

// this is the call back of the next frame button
void HelloWorld::menuNextFrameCallback(CCObject* pSender)
{

    this->unschedule(schedule_selector(HelloWorld::Flip));
    unschedule(schedule_selector(HelloWorld::UpdateProgress));

    //CCScene* scene = HelloWorld::transScene(-1000);
    //CCTransitionSlideInT* tran = CCTransitionSlideInT::create(0.5, scene);
    //CCDirector::sharedDirector()->replaceScene(tran);
}


void HelloWorld::menuAddFrameCallback(CCObject* pSender)
{
    this->unschedule(schedule_selector(HelloWorld::Flip));
    unschedule(schedule_selector(HelloWorld::UpdateProgress));
    if (pCloseItemAddButton->getScale()==1) {
        // enlarge the del button for the 1st time
        pCloseItemAddButton->setScale(2);
        return;
    }
    // revert del button back to normal size for the 2nd time, and here really del the frame
    pCloseItemAddButton->setScale(1);
    int iterLoc = location;
    CCString* cca;
    
    std::string ready_to_insert = CCString::create("[Add]")->getCString();
    do
    {
        std::string a = CCUserDefault::sharedUserDefault()->getStringForKey(CCString::createWithFormat("%d", iterLoc)->getCString(), "");
        CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%d", iterLoc)->getCString(), ready_to_insert);
        ready_to_insert = a;
        iterLoc = iterLoc - 1000;
        cca = CCString::create(ready_to_insert);
    }while (cca!=NULL && strcmp(cca->getCString(), "")!=0);
    CCLOG("????%d", location);
    
    CCScene* scene = HelloWorld::transScene(location);
    CCTransitionSlideInB* tran = CCTransitionSlideInB::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

// this is the call back of the next frame button
void HelloWorld::menuDelFrameCallback(CCObject* pSender)
{
    this->unschedule(schedule_selector(HelloWorld::Flip));
    unschedule(schedule_selector(HelloWorld::UpdateProgress));
    
    if (pDelItemNextFrame->getScale()==1) {
        // enlarge the del button for the 1st time
        pDelItemNextFrame->setScale(2);
        return;
    }
    
    // revert del button back to normal size for the 2nd time, and here really del the frame
    pDelItemNextFrame->setScale(1);
    
    CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%d", location)->getCString(), "");
    
    int iterLoc = location;
    
    CCString* cca;
    do
    {
        std::string a = CCUserDefault::sharedUserDefault()->getStringForKey(CCString::createWithFormat("%d", iterLoc-1000)->getCString(), "");
        
        CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%d", iterLoc)->getCString(), a);
        
        iterLoc -= 1000;
        
        cca = CCString::create(a);

    }while (cca!=NULL && strcmp(cca->getCString(), "")!=0);
    
    CCScene* scene = HelloWorld::transScene(location);
    CCTransitionSlideInB* tran = CCTransitionSlideInB::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
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

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

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
    } else if(abs(endY)<visibleSize.height/2 && abs(endX) > abs(endY))
    {
        if(abs(endX)>15)
        {
            if(endX>15)
            {
                doLeft();
            }
            else
            {
                doRight();
            }
            finishTransAction(NULL);
        }
    }
    this->unschedule(schedule_selector(HelloWorld::Flip));
}

void HelloWorld::Flip(float dt)
{
    CCLOG("===Auto Flip=== to location:%d", location-1000);

    // No need to go down for current frame anymore
    //doDown();
    
    unscheduleUpdate();

    CCScene* scene = HelloWorld::transScene(location-1000);
    
    CCLOG("===Auto Fli111=== to location:%d", location-1000);
    CCString* cca = getStringFromSavedLocation(location-1000);
    CCLOG("===Auto cca=== to location:%s", cca->getCString());
    // if it is auto trans, do not show empty cells
    if(cca==NULL || strcmp(cca->getCString(), "")==0)
    {
        CCTransitionSlideInT* tran = CCTransitionSlideInT::create(0.3, scene);
        CCDirector::sharedDirector()->replaceScene(tran);
        return;
    }
    
    CCTransitionSlideInB* tran = CCTransitionSlideInB::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
    return;
}

void HelloWorld::refreshFrameByLocation(int inputLocation)
{
    location = inputLocation;
    pTestLayer->setLocation(location);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    float hscale = s.height/12/pTestLayer->m_pTextField->getContentSize().height;
    //float wscale = s.width/pTestLayer->m_pTextField->getContentSize().width;
    //pTestLayer->m_pTextField->setScale(hscale>wscale?wscale:hscale);
    
    pTestLayer->m_pTextField->setScale(hscale);
    
    //finishTransAction(NULL);
}

int HelloWorld::getCurrentLocation()
{
    return location;
}

bool HelloWorld::setStringToSavedLoaction(CCString* myccstring, int loc)
{
    if(myccstring!=NULL && strcmp(myccstring->getCString(),"")!=0)
    {
        CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%d", loc)->getCString(), myccstring->getCString());
        return true;
    }
    return false;
}

CCString* HelloWorld::getStringFromSavedLocation(int loc)
{
    std::string a = CCUserDefault::sharedUserDefault()->getStringForKey(CCString::createWithFormat("%d", loc)->getCString(), "");
    CCString* cca = CCString::create(a);
    return cca;
}

void HelloWorld::show4Grid(int type = 1111)
{
    int a,b,c,d;
    a=type/1000;
    b=type%1000/100;
    c=type%100/10;
    d=type%10;
    
    CCString* temp1 = CCString::createWithFormat("%d",a);
    CCString* temp2 = CCString::createWithFormat("%d",b);
    CCString* temp3 = CCString::createWithFormat("%d",c);
    CCString* temp4 = CCString::createWithFormat("%d",d);
    
    CCLabelTTF* la = CCLabelTTF::create(temp1->getCString(), FONT_NAME, 28);
    CCLabelTTF* lb = CCLabelTTF::create(temp2->getCString(), FONT_NAME, 28);
    CCLabelTTF* lc = CCLabelTTF::create(temp3->getCString(), FONT_NAME, 28);
    CCLabelTTF* ld= CCLabelTTF::create(temp4->getCString(), FONT_NAME, 28);

    ccColor3B color = {255, 255, 255};
    la->setColor(color);
    
    la->setPosition(ccp(200,100));
    lb->setPosition(ccp(250,100));
    lc->setPosition(ccp(200,150));
    ld->setPosition(ccp(250,150));
    addChild(la,1);
    addChild(lb,1);
    addChild(lc,1);
    addChild(ld,1);
}

void HelloWorld::showPostionOnFrame()
{
    CCString* myposition = CCString::createWithFormat("%d of %d",location*-1/1000+1,location*-1%1000+1);
    if( positionLabel!=NULL)
    {
        CC_SAFE_DELETE(positionLabel);
    }
    positionLabel=CCLabelTTF::create(myposition->getCString(), FONT_NAME, 18);
    
    ccColor3B color = { 255, 255, 255};
    positionLabel->setColor(color);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //positionLabel->setPosition(ccp(visibleSize.width-positionLabel->getContentSize().width, visibleSize.width-positionLabel->getContentSize().height));
    positionLabel->setPosition(ccp(100,visibleSize.height-positionLabel->getContentSize().height));
    
    addChild(positionLabel, 1);
    
}


bool HelloWorld::showhideMiddleWord(bool doornot)
{
    pTestLayer->setVisible(doornot);
    return true;
}

bool HelloWorld::checkifEnvelopeNeeded(CCString* cca)
{
    if(cca==NULL || strcmp(cca->getCString(), "")==0)
    {
        return false;
    } else if(strstr(cca->getCString(), "#envelope#")!=NULL) //(c) c means countdown
    {
        std::string anticipateTimeSec = cca->m_sString.substr(10,-1);
        CCString* ccb = CCString::create(anticipateTimeSec);
        
        createEvelopeCounterdown(ccb);
        return true;
    }
    return false;
}

bool HelloWorld::checkIfBallsNeeded(CCString* cca)
{
    if(cca==NULL || strcmp(cca->getCString(), "")==0)
    {
        return false;
    } else if(strstr(cca->getCString(), "#balls#")!=NULL) //(c) c means countdown
    {
        //CCLOG("*****BINGO BALL");
        showhideMiddleWord(false);
        createBallsAnim();
    }
    return true;
}

void HelloWorld::createEvelopeCounterdown(CCString* ccb)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    
    CCMenuItemImage* pEnvButton = CCMenuItemImage::create(
                                                  "envelope.png",
                                                  "envelope.png",
                                                  this,
                                                  menu_selector(HelloWorld::menuEnvCanReadCallback));
    
    pEnvButton->setPosition(ccp(visibleSize.width/2,visibleSize.height/2));
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pEnvButton, NULL);
    pMenu->setPosition(CCPointZero);
    
    this->addChild(pMenu, 1,TAG_ENVELOP_PNG);
    moveSideMiddleWord();

    createEnvelopeCountDownLable(ccb);
    
    return;
}


void HelloWorld::menuEnvCanReadCallback(CCObject* pSender)
{
    this->unschedule(schedule_selector(HelloWorld::Flip));
    unschedule(schedule_selector(HelloWorld::UpdateProgress));
    CCLOG("=== Enve button activated");
    CCMenu* pMenu = (CCMenu*) this->getChildByTag(TAG_ENVELOP_PNG);
    pMenu->setVisible(false);
    moveCenterMiddleWord();
    
    std::string a = CCUserDefault::sharedUserDefault()->getStringForKey(CCString::createWithFormat("%dcontent", location)->getCString(), "");
    CCString* cca = CCString::create(a);
    pTestLayer->m_pTextField->setString(cca->getCString());
    
    
    //CCMenu* pMenu =  = (CCMenu*)this->getChildByTag(TAG_ENVELOP_PNG);
    //pMenu
}

//#deprecated
bool HelloWorld::moveSideMiddleWord()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    pTestLayer->m_pTextField->setPosition(ccp(s.width/2, s.height /5 + 50));
    pTestLayer->m_pTextField->setScale(0.5);
    
    return true;
}


bool HelloWorld::moveCenterMiddleWord()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    pTestLayer->m_pTextField->setPosition(ccp(s.width/2, s.height /2 + 50));
    pTestLayer->m_pTextField->setScale(1);
    
    return true;
}

void HelloWorld::printTime(time_t deltatime)
{
    struct tm *tm;
    tm = localtime(&deltatime);
    
    int day = tm->tm_mday;
    int hour =tm->tm_hour;
    int min = tm->tm_min;
    int sec =tm->tm_sec;
    CCLOG("printtime: %d d %d h %d m %d s",day, hour ,min, sec);
}

void HelloWorld::createEnvelopeCountDownLable(CCString* ccb)
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
    time_t uncovertime = atol(ccb->getCString());
    
    printTime(uncovertime);
    printTime(timep);
    
    time_t deltatime = uncovertime - timep;
    
    CCLOG("delta %ld",deltatime);
    int day = deltatime / 3600 / 24;
    int hour = deltatime % (3600 * 24) / 3600;
    int min = deltatime % (3600) / 60;
    int sec =deltatime % 60;
    
    CCString* deltaccstring;
    if (deltatime<=0)
    {
        deltaccstring = CCString::createWithFormat("ok, can open now");
        CCSprite* envelop_png = (CCSprite*)this->getChildByTag(TAG_ENVELOP_PNG);
        CCActionInterval * move = CCFadeIn::create(0.5);
        CCActionInterval * delaytime = CCDelayTime::create(0.2);
        CCActionInterval * move1 = CCFadeOut::create(0.5);
        CCActionInterval * seq= CCSequence::create(move,delaytime,move1,NULL);
        CCRepeatForever * rep = CCRepeatForever::create(seq);
        
        pTestLayer->m_pTextField->setScale(0.3);
        
        envelop_png->runAction(rep);
        
    }
    else if(day!=0 && hour!=0)
    {
        deltaccstring = CCString::createWithFormat("%dd%dh\nto open...",day, hour);
    } else if(day==0 && hour!=0)
    {
        deltaccstring = CCString::createWithFormat("%dh%dm\nto open...",hour,min);
    } else if(day==0 && hour==0 && min!=0)
    {
        deltaccstring = CCString::createWithFormat("%dm%ds\nto open...",min,sec);
    } else if(day==0 && hour==0 && min==0 && sec!=0)
    {
        deltaccstring = CCString::createWithFormat("%ds\nto open...",sec);
    } else
    {
        deltaccstring = CCString::createWithFormat("ok, can open now");
    }
    
    CCLabelTTF* deltalabel = CCLabelTTF::create(deltaccstring->getCString(), FONT_NAME, 40);
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    deltalabel->setPosition(ccp(visibleSize.width/2,visibleSize.height/5*4));
    this->addChild(deltalabel);
}

void HelloWorld::createBallsAnim()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    for (int i = 0; i < 10; i++)
    {
        CCLabelTTF* ballItem = CCLabelTTF::create("BALL", FONT_NAME, 40);
        ballItem->setColor(ccc3(155+100*CCRANDOM_0_1(),155+100*CCRANDOM_0_1(),155+100*CCRANDOM_0_1()));
        
        ballItem->setPosition(ccp(visibleSize.width/2-(5-i)*100,visibleSize.height));
        
        // start to build the action
        int targetx = visibleSize.width/2-(5-i)*100;
        int targety = visibleSize.height/2;
        
        CCActionInterval * move = CCMoveTo::create(1, ccp(targetx,targety));
        
        //CCLOG("----time--->%f",0.1*abs(i-5));
        
        CCActionInterval * delaytime = CCDelayTime::create(0.1*abs(i-5)+0.5);
        CCFiniteTimeAction * seq= CCSequence::create(delaytime, CCEaseElasticOut::create(move),NULL);
        
        ballItem->runAction(seq);
        
        this->addChild(ballItem,1);
    }
    return;
}

bool HelloWorld::checkIfSanAnimNeeded(CCString* cca)
{
    if(cca==NULL || strcmp(cca->getCString(), "")==0)
    {
        return false;
    } else if(strstr(cca->getCString(), "#san#")!=NULL) //(c) c means countdown
    {
        CCLOG("*****BINGO SAN");
        showhideMiddleWord(false);
        
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        
        float centx,centy;
        
        CCArray* array = CCArray::create();
        for (int i = 0; i < 10; i++)
        {
            CCLabelTTF* sanItem = CCLabelTTF::create("散", FONT_NAME, 20);
            
            sanItem->setPosition(ccp(visibleSize.width/2-sanItem->getContentSize().width,visibleSize.height/2-sanItem->getContentSize().height));
            addChild(sanItem);
            array->addObject(sanItem);
            
            centx = visibleSize.width/2-sanItem->getContentSize().width;
            centy = visibleSize.height/2-sanItem->getContentSize().height;
        }
        
        for (int i = 0; i < 10; i ++)
        {
            CCLabelTTF* sp = (CCLabelTTF*)array->objectAtIndex(i);
            float targetx, targety;
            targetx = (float)centx + 200.0*cos((float)i*0.628);
            targety = (float)centy + 200.0*sin((float)i*0.628);
            
            CCLOG("x,y %f,%f", cos(0.628*i), sin(0.628*i));
            
            CCActionInterval * move = CCMoveTo::create(1, ccp(targetx,targety));
            CCActionInterval * scale = CCScaleTo::create(1, 3);
            CCActionInterval * rotate = CCRotateBy::create(3, -36*i+90+1080);
            CCFiniteTimeAction * spawn =CCSpawn::create(move,scale,rotate,NULL);
            
            CCActionInterval * delaytime = CCDelayTime::create(0.5);
            CCFiniteTimeAction * seq= CCSequence::create(delaytime,spawn,NULL);
            
            sp->runAction(seq);
        }
        
        return true;
    }
}

bool HelloWorld::checkIfHalfCompNeeded(CCString* cca)
{
    //CCLOG("check need HALF?:%s", cca->getCString());
    if(cca==NULL || strcmp(cca->getCString(), "")==0)
    {
        return false;
    } else if(strstr(cca->getCString(), "#half#")!=NULL) //(c) c means countdown
    {
        CCLOG("*****BINGO HALF");
        size_t half_start_post = cca->m_sString.find("#half#");
        std::string anticipateTimeSec = cca->m_sString.substr(half_start_post+6,-1);
        CCString* ccb = CCString::create(anticipateTimeSec);
        int left_proporation = ccb->intValue();
        CCLOG("int value ->%d", left_proporation);
        
        createHALFBar(left_proporation);
        
        return true;
    }
}

bool HelloWorld::checkIfHTMLMessengerNeeded(CCString* cca)
{
    //CCLOG("ok here HTML?:%s", cca->getCString());
    
    if(cca==NULL || strcmp(cca->getCString(), "")==0)
    {
        return false;
    } else if(strstr(cca->getCString(), "#html#")!=NULL) //(c) c means countdown
    {

        // 发送html 查询pm25的网页
        if (html_static_dict->objectForKey("pm1")==NULL) {
            cocos2d::extension::CCHttpRequest* request = new cocos2d::extension::CCHttpRequest();
            request->setUrl("http://www.baidu.com/s?wd=pm25");
            request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpGet);
            std::vector<std::string> headers;
            headers.push_back("Content-Type: text/html; charset=utf-8");
            request->setHeaders(headers);
            const char* postData = "catalog=2&pageIndex=1&pageSize=5";
            request->setRequestData(postData ,strlen(postData));
            request->setResponseCallback(this, callfuncND_selector(HelloWorld::onHttpRequestCompleted));
            request->setTag("Post_My_Data");
            cocos2d::extension::CCHttpClient::getInstance()->send(request);
            request->release();
        } else
        {

            createHTMLBar();
            
        }
        
        return true;
    }
    
    return true;
}

bool HelloWorld::checkIfPaceNeeded(cocos2d::CCString *cca)
{
    if(strstr(cca->getCString(), "#pace#")!=NULL) //(c) c means countdown
    {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        int positionX = visibleSize.width/2;
        int positionY = visibleSize.height*3/4;
        
        touchForPaceEnabled = true;
        
        CCString* temp = CCString::createWithFormat("%d",allDistanceByCm/100);
        CCLabelTTF* pmlabel1 = CCLabelTTF::create(temp->getCString(), FONT_NAME, 40);
        pmlabel1->setPosition(ccp(positionX, positionY));
        this->addChild(pmlabel1, 1, TAG_PACE_LABEL);
    }

}

void HelloWorld::refreshPaceLabel()
{
    CCString* temp = CCString::createWithFormat("%d",allDistanceByCm/100);
    CCLabelTTF* pmlabel1 = (CCLabelTTF*)this->getChildByTag(TAG_PACE_LABEL);
    pmlabel1->setString(temp->getCString());
}

struct tm* HelloWorld::getCurrentTMStruct()
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
    return tm;
}

bool HelloWorld::checkIfDailyNeeded(CCString* cca)
{
    if(cca==NULL || strcmp(cca->getCString(), "")==0)
    {
        return false;
    } else if(strstr(cca->getCString(), "#daily#")!=NULL) //(c) c means countdown
    {
        
        CCLOG("need daily?");
        
        struct tm *tm=getCurrentTMStruct();
        int year = tm->tm_year + 1900;
        int month = tm->tm_mon + 1;
        int day = tm->tm_mday;
        int hour=tm->tm_hour;
        int min=tm->tm_min;
        int second=tm->tm_sec;
        
        CCString *str1 = CCString::createWithFormat("%d-%d-%d %d:%d:%d",year,month,day,hour,min,second);
        
        CCLabelTTF* pmlabel1 = CCLabelTTF::create(str1->getCString(), FONT_NAME, 20);
        pmlabel1->setPosition(ccp(200, 250));
        pmlabel1->setTag(TAG_DAILY);
        this->addChild(pmlabel1,1);
        
        this->unscheduleUpdate();
        this->scheduleUpdate();

        return true;
    }
    return false;
}

bool HelloWorld::checkIfCounterNeeded(CCString* cca)
{
    CCLOG("check need HALF?:%s", cca->getCString());
    if(cca==NULL || strcmp(cca->getCString(), "")==0)
    {
        return false;
    } else if(strstr(cca->getCString(), "#counter#")!=NULL) //(c) c means countdown
    {
        CCLOG("*****BINGO counter");
        createUpDownButton();
        showhideMiddleWord(false);
        
        return true;
    }
    return false;
}

bool HelloWorld::checkIfProgressBarNeeded(CCString* cca)
{
    hideProgressBar();
    
    //CCLOG("ok here:%s", cca->getCString());

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
    } else if(strstr(cca->getCString(), "#time#")!=NULL) //(c) c means countdown
    {
        std::string anticipateTimeSec = cca->m_sString.substr(3,-1);
        CCString* ccb = CCString::create(anticipateTimeSec);
        // OK, we got the anticipated seconds
        std::string split = ",";
        CCArray* strs = splitEx(anticipateTimeSec, split);

        if(strs->count()==2)
        {
            CCLOG("--Time Str Found, Part1>%s", ((CCString*)strs->objectAtIndex(0))->getCString());
            CCLOG("--Time Str Found, Part2>%s", ((CCString*)strs->objectAtIndex(1))->getCString());

            createProgressBar();
        }
        return true;
    } else {
        unscheduleUpdate();
        hideProgressBar();
        //if(progress1!=NULL)
        //{
        //    progress1->setVisible(false);
        //    progressbgSprite->setVisible(false);
        //    numsTTF->setVisible(false);
        //    numsTTF1->setVisible(false);
        //}
    }
    return true;
}

bool HelloWorld::finishTransAction(CCNode* pSender)
{
    CCLOG("Buffer Location:%d",location);

    CCString* cca = getStringFromSavedLocation(location);

    pTestLayer->m_pTextField->setString(cca->getCString());

    CCLOG("Buffer Str:%s",cca->getCString());

    pTestLayer->setVisible(true);

    return checkIfProgressBarNeeded(cca);
}

void HelloWorld::showFirstFrameIcon()
{
    CCLOG("==========pos:%d", location);
    if(location<=-1000)
    {
        return;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    //CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    //创建一个进度条精灵，这个是2.0以后api变了
    int positionX = visibleSize.width/4;
    int positionY = visibleSize.height*3/4;
    
    CCSprite* spt = CCSprite::create("button_blue_first.png");
    
    spt->setPosition(ccp(positionX,positionY));
    
    this->addChild(spt, 1);
}


void HelloWorld::hideProgressBar()
{
    unscheduleUpdate();
    if(progress1!=NULL)
    {
        progress1->setVisible(false);
    }
    if(progressbgSprite!=NULL)
    {
        progressbgSprite->setVisible(false);
    }
    if(numsTTF!=NULL)
    {
        numsTTF->setVisible(false);
    }
    if(numsTTF1!=NULL)
    {
        numsTTF1->setVisible(false);
    }
}

void HelloWorld::createUpDownButton()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    int positionX = visibleSize.width*3/4;
    int positionY = visibleSize.height/2;
    
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCMenuItemImage* pUpButton = CCMenuItemImage::create(
                                                "upbutton.png",
                                                "upbutton2.png",
                                                this,
                                                menu_selector(HelloWorld::menuUpButtonCallback));
    
    
    CCMenuItemImage* pDownButton= CCMenuItemImage::create(
                                                "downbutton.png",
                                                "downbutton2.png",
                                                this,
                                                menu_selector(HelloWorld::menuDownButtonCallback));

    
    pUpButton->setPosition(ccp(origin.x + positionX  ,
                                       origin.y + positionY+50));
    pDownButton->setPosition(ccp(origin.x + positionX  ,
                                       origin.y + positionY-50));
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pUpButton, pDownButton, NULL);
    pMenu->setPosition(CCPointZero);
    
    int curloc = getCurrentLocation();
    
    CCString* mynumstr = getStringFromSavedLocation(curloc);
    
    char *a = strstr(mynumstr->getCString(), "#counter#");
    
    a = a + 9;
    
    CCLabelTTF* counterNumberLabel;
    
    CCLOG("------>%d<---", strcmp(a, ""));

    if(a==NULL || !isdigit(*a))
    {
        CCString* temp = CCString::create("0");
        counterNumberLabel = CCLabelTTF::create(temp->getCString(), FONT_NAME, 150);
    } else
    {
        counterNumberLabel = CCLabelTTF::create(a, FONT_NAME, 150);
    }
    
    
    counterNumberLabel->setPosition(ccp(visibleSize.width/2,visibleSize.height/2));
    addChild(counterNumberLabel, 1, TAG_COUNTER_LABEL);
    
    this->addChild(pMenu, 1);
}

void HelloWorld::createHALFBar(int left_proportion)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    int positionX = visibleSize.width/2;
    int positionY = visibleSize.height*3/4;
    
    progressbgSprite=CCSprite::create("cloud-upload-bg.png");
    progressbgSprite->setPosition(ccp(positionX,positionY));
    
    this->addChild(progressbgSprite, 1);
    int right_proportion = 100 - left_proportion;

    CCString* left_str = CCString::createWithFormat("%d%%", left_proportion);
    
    CCString* right_str = CCString::createWithFormat("%d%%", right_proportion);
    
    CCLabelTTF* pmlabel1 = CCLabelTTF::create(left_str->getCString(), FONT_NAME, 20);
    CCLabelTTF* pmlabel2 = CCLabelTTF::create(right_str->getCString(), FONT_NAME, 20);
    pmlabel1->setPosition(ccp(positionX/2, positionY));
    pmlabel2->setPosition(ccp(positionX*3/2, positionY));
    

    CCSprite *progressSprite=CCSprite::create("cloud-upload.png");
    
    progress1=CCProgressTimer::create(progressSprite);
    
    progress1->setType(kCCProgressTimerTypeBar);
    
    progress1->setPosition(ccp(positionX, positionY));
    
    progress1->setBarChangeRate(ccp(1, 0));
    progress1->setMidpoint(ccp(0, 0));
    
    progress1->setPercentage(left_proportion);
    
    this->addChild(progress1, 1);

    
    this->addChild(pmlabel1, 1);
    this->addChild(pmlabel2, 1);

}

void HelloWorld::createHTMLBar()
{
    CCLOG("CRm   HTML BAR");
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    int positionX = visibleSize.width/2;
    int positionY = visibleSize.height*3/4;
    
    //numsTTF=CCLabelTTF::create("0000000", FONT_NAME, 18);
    //numsTTF->setPosition(ccp(positionX, positionY-100));
    
    //this->addChild(numsTTF, 1);
    
    
    CCString* pm25_1 = (CCString*)html_static_dict->objectForKey("pm1");
    
    CCString* pm25_2 = (CCString*)html_static_dict->objectForKey("pm2");
    
    CCLabelTTF* pmlabel1 = CCLabelTTF::create(pm25_1->getCString(), FONT_NAME, 40);
    CCLabelTTF* pmlabel2 = CCLabelTTF::create(pm25_2->getCString(), FONT_NAME, 40);
    pmlabel1->setPosition(ccp(positionX, positionY+20));
    pmlabel2->setPosition(ccp(positionX, positionY-20));

    
    CCLOG("ok here    3 pa1:%s pa2:%s", pm25_1->getCString(), pm25_2->getCString());
    
    this->addChild(pmlabel1, 1);
    this->addChild(pmlabel2, 1);

}

void HelloWorld::createProgressBar()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    //创建一个进度条精灵，这个是2.0以后api变了
    //progressbgSprite=CCSprite::create("time_slot_hd.png");
    progressbgSprite=CCSprite::create("cloud-upload-bg.png");

    int positionX = visibleSize.width/2;
    int positionY = visibleSize.height/4-progressbgSprite->getContentSize().height/2;

    progressbgSprite->setPosition(ccp(positionX,positionY
                                      ));

    this->addChild(progressbgSprite, 1);


    //CCSprite *progressSprite=CCSprite::create("time_bars_hd.png");
    CCSprite *progressSprite=CCSprite::create("cloud-upload.png");

    progress1=CCProgressTimer::create(progressSprite);

    progress1->setType(kCCProgressTimerTypeBar);

    progress1->setPosition(ccp(positionX, positionY));

    //进度动画运动方向，可以多试几个值，看看效果
    progress1->setMidpoint(ccp(0, 0));

    //进度条宽高变化
    progress1->setBarChangeRate(ccp(1, 0));

    progress1->setPercentage(0);

    this->addChild(progress1, 1);

    numsTTF=CCLabelTTF::create("0", FONT_NAME, 18);


    numsTTF->setPosition(ccp(positionX, positionY-progressbgSprite->getContentSize().height));


    this->addChild(numsTTF, 1);


    numsTTF1=CCLabelTTF::create("0", FONT_NAME, 18);

    numsTTF1->setPosition(ccp(positionX, positionY+50));


    numsTTF1->setString("今年过去了");
    this->addChild(numsTTF1, 1);

    struct tm *tm=getCurrentTMStruct();
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
    struct tm *tm=getCurrentTMStruct();
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;
    int hour=tm->tm_hour;
    int min=tm->tm_min;
    int second=tm->tm_sec;

    secPassedSinceMidNight += t;
    percentTodayOfYear = (tm->tm_yday*100.0+(100.0/(24*60*60)*(secPassedSinceMidNight)))/365.0;
    CCString *str1 = CCString::createWithFormat("%.10f%%",percentTodayOfYear);

    if(numsTTF!=NULL)
    {
        numsTTF->setString(UTEXT(str1->getCString()));
        progress1->setPercentage(percentTodayOfYear);
    }
    
    CCLabelTTF* lb = (CCLabelTTF*)this->getChildByTag(TAG_DAILY);
    str1 = CCString::createWithFormat("%d-%d-%d %d:%d:%d",year,month,day,hour,min,second);
    if(lb!=NULL)
    {
        lb->setString(str1->getCString());
    }
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

void HelloWorld::menuUpButtonCallback(CCObject* pSender)
{
    this->unschedule(schedule_selector(HelloWorld::Flip));
    unschedule(schedule_selector(HelloWorld::UpdateProgress));
    CCLOG("=== UP BTN activated");
    
    CCLabelTTF* lb = (CCLabelTTF*)this->getChildByTag(TAG_COUNTER_LABEL);
    
    CCString* str1 = CCString::create(lb->getString());
    
    int tempint = str1->intValue() + 1;
    str1->release();
    str1 = CCString::createWithFormat("%d", tempint);
    
    if(lb!=NULL)
    {
        lb->setString(str1->getCString());
        
        CCString* result = CCString::createWithFormat("#counter#%s",str1->getCString());
        
        setStringToSavedLoaction(result, getCurrentLocation());
    }
}

void HelloWorld::menuDownButtonCallback(CCObject* pSender)
{
    this->unschedule(schedule_selector(HelloWorld::Flip));
    unschedule(schedule_selector(HelloWorld::UpdateProgress));
    CCLOG("=== DOWN BTN activated");
    
    CCLabelTTF* lb = (CCLabelTTF*)this->getChildByTag(TAG_COUNTER_LABEL);
    
    CCString* str1 = CCString::create(lb->getString());
    
    int tempint = str1->intValue() - 1;
    str1->release();
    str1 = CCString::createWithFormat("%d", tempint);
    
    if(lb!=NULL)
    {
        lb->setString(str1->getCString());
        
        CCString* result = CCString::createWithFormat("#counter#%s",str1->getCString());
        
        setStringToSavedLoaction(result, getCurrentLocation());
    }
    
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
        //CCLOG("TextFieldTTFActionTest:CCTextFieldTTF attachWithIME");
        pTextField->attachWithIME();
    }
    else
    {
        // TextFieldTTFTest not be clicked
        //CCLOG("TextFieldTTFActionTest:CCTextFieldTTF detachWithIME");
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
    
    if (m_pTextField == NULL) {
        m_pTextField = CCTextFieldTTF::textFieldWithPlaceHolder("<click here for input>",
                                                                FONT_NAME,
                                                                FONT_SIZE);
        
    }
    addChild(m_pTextField);

    float hscale = s.height/4;

    float wscale = s.width/m_pTextField->getContentSize().width;

    //m_pTextField->setScale(hscale>wscale?wscale:hscale);
    std::string a = CCUserDefault::sharedUserDefault()->getStringForKey(CCString::createWithFormat("%d", this->curLocation)->getCString(), "");
    CCString* cca = CCString::create(a);
    m_pTextField->setString(cca->getCString());

    m_pTextField->setDelegate(this);

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

bool TextFieldTTFActionTest::checkIfNeedHandleInputString(CCTextFieldTTF * pSender)
{
    // pSender is the text box of center label
    // pSender->setString("abc123");
    CCString* cca = CCString::create(pSender->getString());
    if(cca==NULL || strcmp(cca->getCString(), "")==0)
    {
        return false;
    } else if(strstr(cca->getCString(), "#envelope#")!=NULL) //(c) c means countdown
    {
        CCLOG("ok, after input, we got %s", pSender->getString());
        
        std::string anticipateTimeSec = cca->m_sString.substr(10,-1);
        
        CCString* ccb = CCString::create(anticipateTimeSec);
        char* origin = (char *)ccb->getCString();
        
        char* blank = origin+ccb->length()-1;
        
        CCString* eve_content = CCString::create("");
        
        if(strstr(origin," ") != NULL)
        {
            blank = strstr(origin," ");
            int from_start_to_blank_len = (int)(blank - origin);
            anticipateTimeSec = ccb->m_sString.substr(0,from_start_to_blank_len);
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
        CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%dcontent", curLocation)->getCString(), eve_content->getCString());
        
        char *result_day_array = NULL;
        char *result_hour_array = NULL;
        char *result_min_array = NULL;
        char *result_sec_array = NULL;
        
        //CCLOG("---- - - - %s %s", strstr(origin,"d"), *blank);
        if(strstr(origin,"d") != NULL )// || strstr(a,"m") != NULL) // a long type or 3d5h3m1s
        {
            CCLOG("day detected");
            d = strstr(origin,"d");
            size_t len = d - origin;
            result_day_array = (char*)malloc(sizeof(char)*(len+1));
            strncpy(result_day_array, origin, len);
            result_day_array[len] = '\0';
        }
        if(strstr(origin,"h") != NULL )
        {
            CCLOG("hour detected");

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
            CCLOG("min detected");

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
            CCLOG("sec detected");

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
        
        CCLOG("parse result");
        CCLOG("%s",result_day_array);
        CCLOG("%s",result_hour_array);
        CCLOG("%s",result_min_array);
        CCLOG("%s",result_sec_array);
        
        int nDay=0, nHour=0, nMin=0, nSec=0;
        if(result_day_array!=NULL)nDay = atoi(result_day_array);
        if(result_hour_array!=NULL)nHour = atoi(result_hour_array);
        if(result_min_array!=NULL)nMin = atoi(result_min_array);
        if(result_sec_array!=NULL)nSec = atoi(result_sec_array);
        
        CCLOG("day: %d", nDay);
        CCLOG("h: %d", nHour);
        CCLOG("m: %d", nMin);
        CCLOG("s: %d", nSec);
        
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
        
        
        CCLOG("printtime: %d %d:%d:%d",tm->tm_mday, tm->tm_hour ,tm->tm_min, tm->tm_sec);
        
        CCLOG("delta: %d %d:%d:%d", nDay, nHour , nMin, nSec);
        
        timep+=nSec;
        timep+=nMin*60;
        timep+=nHour*60*60;
        timep+=nDay*60*60*24;
        
        tm = localtime(&timep);
        
        CCLOG("ok %d", tm->tm_mday);
        CCLOG("%d", tm->tm_hour);
        CCLOG("%d", tm->tm_min);
        
        
        CCLOG("printtime: %d %d:%d:%d",tm->tm_mday, tm->tm_hour ,tm->tm_min, tm->tm_sec);

        
        CCString* result = CCString::createWithFormat("#envelope#%ld", timep);
        pSender->setString(result->getCString());
        // now we already have
        return true;
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
            CCLOG("- - - - %s", pSender->getString());
            checkIfNeedHandleInputString(pSender);
            
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
    // now deprecated
    if (false && pSender->getCharCount() % m_nCharLimit > (m_nCharLimit-3))
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
    //location += 1000;
    //pTestLayer->setLocation(location);

    //If you are already at the 1st frame
    // Stop user from adding a new one before 1st
    if(location+1000>0)
        return false;

    int mannualFlp = 1;

    CCScene* scene = HelloWorld::transScene(location+1000, mannualFlp);
    CCTransitionSlideInT* tran = CCTransitionSlideInT::create(0.3, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
    
    return true;
}

bool HelloWorld::doDown()
{
    CCLOG("doDown");
    //location -= 1000;
    //pTestLayer->setLocation(location);

    int mannualFlp = 1;

    CCScene* scene = HelloWorld::transScene(location-1000, mannualFlp);
    CCTransitionSlideInB* tran = CCTransitionSlideInB::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

bool HelloWorld::doLeft()
{
    CCLOG("doLeft");
    
    int mannualFlp = 1;
    
    // when do left delete the current, as set to ""
    //CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%d", location)->getCString(), "");
    
    CCScene* scene = HelloWorld::transScene(location%1000-1, mannualFlp);
    CCTransitionSlideInR* tran = CCTransitionSlideInR::create(0.2, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

bool HelloWorld::doRight()
{
    CCLOG("doRight");
    
    int mannualFlp = 1;
    // when do left delete the current, as set to ""
    //CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%d", location)->getCString(), "");
    
    if(location%1000+1 > 0)
    {
        return false;
    }
    
    CCScene* scene = HelloWorld::transScene(location%1000+1, mannualFlp);
    CCTransitionSlideInL* tran = CCTransitionSlideInL::create(0.2, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

void TextFieldTTFActionTest::setLocation( int location )
{
    this->curLocation = location;
}
