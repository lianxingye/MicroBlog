//
//  BallCloudLayer.cpp
//  MicroBlog
//
//  Created by tosoth on 12/8/15.
//
//

#include "BallCloudLayer.h"

#define TAG_LABEL_LOG 1111

#define FONT_NAME                       "Thonburi"//"FZXS12--GB1-0.ttf"
//"FZXS12--GB1-0.ttf"
#define FONT_SIZE                       35


BallItem::~BallItem(void){
}

BallItem::BallItem(void){
}


BallItem* BallItem::createBallItem() {
    BallItem * sprite = new BallItem();
    if (sprite && sprite->init()) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void BallCloudLayer::setStringArraySource(CCArray* input) {
    stringArr = input;
}


void BallCloudLayer::onEnter()
{
    CCLayer::onEnter();
    
    angleX=0.02;
    angleY=0.02;
    
    secPassedSinceMidNight=0;
    
    ballGp = NULL;
    
    ballGp = CCArray::create();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    if (stringArr == NULL || stringArr->count() == 0) {
        
        CCLog("1111");
        
        stringArr->addObject(CCString::create("勿在浮沙筑高台"));
        stringArr->addObject(CCString::create("最后一轮测试"));
        stringArr->addObject(CCString::create("下个做游戏"));
        stringArr->addObject(CCString::create("English"));
        stringArr->addObject(CCString::create("MicroBlog"));
        stringArr->addObject(CCString::create("准备上线"));
        stringArr->addObject(CCString::create("时间本月内"));
        stringArr->addObject(CCString::create("修改框架"));
        stringArr->addObject(CCString::create("物联网"));
        stringArr->addObject(CCString::create("遥控的概念"));
        stringArr->addObject(CCString::create("界面要炫"));
        stringArr->addObject(CCString::create("操作快"));
        stringArr->addObject(CCString::create("不卡"));
        stringArr->addObject(CCString::create("不能等了"));
        stringArr->addObject(CCString::create("信心"));
        stringArr->addObject(CCString::create("怎样简单又炫"));
        stringArr->addObject(CCString::create("动以处人 静以处己"));
        stringArr->addObject(CCString::create("AI还没有"));
        stringArr->addObject(CCString::create("高中的事真的会记你一辈子"));
    }
    
    float N = stringArr->count();
    float h = M_PI * (3 - sqrt(5));
    float s = 2 / N;
    
    
    for (int i =0; i<N; i++) {
        BallItem* ttf = BallItem::createBallItem();
        
        CCString* tempstr = (CCString*)stringArr->objectAtIndex(i);
        
        ttf->setString(tempstr->getCString());
        ttf->setFontName(FONT_NAME);
        ttf->setFontSize(FONT_SIZE);
        ttf->setPosition(ccp(visibleSize.width/2-500+100*i,visibleSize.height/2));
        this->addChild(ttf);
        
        ballGp->addObject(ttf);
        
        
        ttf->setTag(i);
        
        float y = i * s - 1 + (s / 2);
        float r = sqrt(1 - y*y);
        float phi = i * h;
        ttf->x = cosf(phi)*r;
        ttf->y = y;
        ttf->z = sinf(phi)*r;
        
        withPoint(ttf->x, ttf->y, ttf->z, ttf);
    }
    count = N;
    scheduleUpdate();
}




void BallCloudLayer::update(float t)
{
    secPassedSinceMidNight+=t;
    
    if (secPassedSinceMidNight>0.1) {
        
        rotateX();
        rotateY();
        secPassedSinceMidNight=0;
    }
}


void BallCloudLayer::rotateX() {
    float cos = cosf(angleX);
    float sin = sinf(angleX);
    
    float N = count;
    BallItem* pObj;
    for (int i =0; i<N; i++) {
        
        pObj = (BallItem*)this->getChildByTag(i);
        
        float y1 = ((BallItem*)pObj)->y * cos - ((BallItem*)pObj)->z * sin;
        float z1 = ((BallItem*)pObj)->z * cos + ((BallItem*)pObj)->y * sin;
        ((BallItem*)pObj)->y = y1;
        ((BallItem*)pObj)->z = z1;
        
        BallItem* ttf = pObj;
        withPoint(ttf->x, ttf->y, ttf->z, ttf);
        
        
    }
    return;
}



void BallCloudLayer::rotateY() {
    
    float cos = cosf(angleY);
    float sin = sinf(angleY);
    
    BallItem* pObj;
    float N = count;
    for (int i =0; i<N; i++) {
        
        pObj = (BallItem*)this->getChildByTag(i);
        float x1 = ((BallItem*)pObj)->x * cos - ((BallItem*)pObj)->z * sin;
        float z1 = ((BallItem*)pObj)->z * cos + ((BallItem*)pObj)->x * sin;
        ((BallItem*)pObj)->x = x1;
        ((BallItem*)pObj)->z = z1;
        
        BallItem* ttf = pObj;
        withPoint(ttf->x, ttf->y, ttf->z, ttf);
    }
    return;
}


void BallCloudLayer::onExit()
{
    
}

void BallCloudLayer::withPoint(float xx, float yy, float zz, BallItem* item)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    float viewSize =  visibleSize.height;
    
    float width = viewSize - item->getContentSize().height*2;
    
    float x = coordinateForNormalizedValue(xx,width);
    float y = coordinateForNormalizedValue(yy,width);
    //view.center = CCPoint(x + item->getContentSize().width, y + item->getContentSize().width);
    item->cocos2d::CCNode::setPosition(x, y);
    
    float z = coordinateForNormalizedValue(zz,width)/1000.0;
    
    float ccc = z + 0.5;
    if (ccc>=1.0) {
        ccc = 1.0;
    }
    item->setScale(ccc);
    
    
    //item->cocos2d::CCNodeRGBA::setColor(ccc3(255*(z+0.4), 255*(z+0.4), 255*(z+0.4)));
    item->setColor(ccc3(255*ccc, 255*ccc, 255*ccc));
    
    
    
}

float BallCloudLayer::coordinateForNormalizedValue(float normalizedValue, float rangeOffset) {
    float half = rangeOffset / 2.f;
    float coordinate = fabs(normalizedValue) * half;
    if (normalizedValue > 0) {
        coordinate += half;
    } else {
        coordinate = half - coordinate;
    }
    return coordinate;
}

void BallCloudLayer::hide()
{
    this->setVisible(false);
    
}

void BallCloudLayer::go()
{
    
}