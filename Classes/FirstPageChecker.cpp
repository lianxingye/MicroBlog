//
//  FirstPage.cpp
//  MicroBlog
//
//  Created by tosoth on 4/14/16.
//
//

#include "FirstPageChecker.h"


CCScene* FirstPageChecker::checkifFirstPageNeeded()
{
    CCScene* scene = NULL;
    
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
    
    // 23:00 - 2:00
    if (tm->tm_hour>=23 || tm->tm_hour<=2)
    {
        scene = HelloWorld::transScene(1,0,0);
        // show "GO TO Sleep Page"
        scene = HelloWorld::transScene(1,0,0);
        CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%d", 1)->getCString(),"Sleep Time");
        
        ((HelloWorld*)scene)->addTextField(CCString::create("23:00-2:00"));
    } else if (tm->tm_hour>=6 && tm->tm_hour<=10)
    {
        scene = HelloWorld::transScene(1,0,0);
        CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("%d", 1)->getCString(),"#html#");
        
        ((HelloWorld*)scene)->addTextField(CCString::create("6:00-10:00"));
    }
    
    return scene;
}
