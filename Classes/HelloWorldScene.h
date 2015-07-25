#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "DateTimeManager.h"
#include "FramDataSet.h"

USING_NS_CC;

class KeyboardNotificationLayer;
class TextFieldTTFActionTest;

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    static cocos2d::CCScene* transScene(int, int, int);
    bool checkIfProgressBarNeeded(CCString* cca);
    void showPostionOnFrame();
    
    bool checkIfPositionEmpty(CCString*);
    
    bool httpRequest(char * tag, char * url, char * requestdata, int requestType);
    
    void show4Grid(int);
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void menuNextFrameCallback(CCObject* pSender);
    void menuDelFrameCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    void addKeyboardNotificationLayer(KeyboardNotificationLayer * pLayer);
    
    bool doUp();
    bool doDown();
    bool doLeft();
    bool doRight();
    bool finishTransAction(CCNode* pSender);
    void Flip(float dt);
    
    void refreshFrameByLocation(int inputLocation);
    int getCurrentLocation();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    CCString* ThreeWordFindFromHTML(CCString* source, CCString* key1, CCString* key2, CCString* key3);
    
    virtual void registerWithTouchDispatcher();
    CCString* getStringFromSavedLocation(int loc);
    CCArray* splitEx(const std::string& src, std::string separate_character);
    
    void createProgressBar();
    
    void hideProgressBar();
    
    void showFirstFrameIcon();
    
    void update(float t);
    void UpdateProgress(float Dt);
    
private:
    int firstX,firstY,endX,endY;
    TextFieldTTFActionTest* pTestLayer;
    int location;
    int locationx, locationy;
    KeyboardNotificationLayer * m_pNotificationLayer;
    
    float flipInterval;
    FramDataSet mySet;
    
    cocos2d::CCProgressTimer* progress1;
    cocos2d::CCSprite* progressbgSprite;
    cocos2d::CCLabelTTF* numsTTF;
    cocos2d::CCLabelTTF* numsTTF1;
    cocos2d::CCLabelTTF* desc1;
    double percentTodayOfYear;
    
    CCMenuItemImage *pDelItemNextFrame;
    
    float secPassedSinceMidNight;
    float secPassedSinceMidNight1;
    cocos2d::CCDictionary* pDict;
    
    cocos2d::CCLabelTTF* positionLabel;
    void onHttpRequestCompleted(cocos2d::CCNode *sender ,void *data);

    
    int typea,typeb,typec,typed;
    
};

//////////////////////////////////////////////////////////////////////////
// KeyboardNotificationLayer for test IME keyboard notification.
//////////////////////////////////////////////////////////////////////////

class KeyboardNotificationLayer : public cocos2d::CCLayer, public cocos2d::CCIMEDelegate
{
public:
    KeyboardNotificationLayer();
    
    virtual std::string subtitle() = 0;
    virtual void onClickTrackNode(bool bClicked) = 0;
    
    virtual void registerWithTouchDispatcher();
    virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& info);
    
    // CCLayer
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
protected:
    cocos2d::CCNode * m_pTrackNode;
    cocos2d::CCPoint  m_beginPos;
};



//////////////////////////////////////////////////////////////////////////
// TextFieldTTFActionTest
//////////////////////////////////////////////////////////////////////////

class TextFieldTTFActionTest : public KeyboardNotificationLayer, public cocos2d::CCTextFieldDelegate
{
    cocos2d::CCAction *          m_pTextFieldAction;
    bool                m_bAction;
    int                 m_nCharLimit;       // the textfield max char limit
    
    int					curLocation;
    
public:
    cocos2d::CCTextFieldTTF *    m_pTextField;
    void callbackRemoveNodeWhenDidAction(cocos2d::CCNode * pNode);
    
    // KeyboardNotificationLayer
    virtual std::string subtitle();
    virtual void onClickTrackNode(bool bClicked);
    
    
    
    // CCLayer
    virtual void onEnter();
    virtual void onExit();
    
    // CCTextFieldDelegate
    virtual bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF * pSender);
    virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF * pSender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF * pSender, const char * delText, int nLen);
    virtual bool onDraw(cocos2d::CCTextFieldTTF * pSender);
    void setLocation( int location );
};



#endif // __HELLOWORLD_SCENE_H__
