#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

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
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    
    void addKeyboardNotificationLayer(KeyboardNotificationLayer * pLayer);
    
    bool doUp();
    bool doDown();
    bool finishTransAction(CCNode* pSender);
    void Flip(float dt);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    
    virtual void registerWithTouchDispatcher();
    CCString* getStringFromSavedLocation(int loc);
    CCArray* splitEx(const std::string& src, std::string separate_character);
    
    void createProgressBar();
    
    void hideProgressBar();
    
    void update(float t);
    
private:
    int firstX,firstY,endX,endY;
    TextFieldTTFActionTest* pTestLayer;
    int location;
    KeyboardNotificationLayer * m_pNotificationLayer;
    
    float flipInterval;
    
    
    cocos2d::CCProgressTimer* progress1;
    cocos2d::CCSprite* progressbgSprite;
    cocos2d::CCLabelTTF* numsTTF;
    cocos2d::CCLabelTTF* numsTTF1;
    cocos2d::CCLabelTTF* desc1;
    double percentTodayOfYear;
    
    float secPassedSinceMidNight;
    float secPassedSinceMidNight1;
    cocos2d::CCDictionary* pDict;
    
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
