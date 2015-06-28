//
//  DateTimeManager.h
//  MicroBlog
//
//  Created by tosoth on 6/27/15.
//
//

#ifndef __MicroBlog__DateTimeManager__
#define __MicroBlog__DateTimeManager__

#include <stdio.h>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

#include "cocos2d.h"

USING_NS_CC;


class DateTimeManage
{
public:
    static time_t getCurrentTime();
    void refreshToCurrentTime();
    static int howManySecSince(time_t tm);
    
private:
    int Day;
    int Month;
    int Year;
    int Hour;
    int Min;
    int Sec;
    
};

#endif /* defined(__MicroBlog__DateTimeManager__) */
