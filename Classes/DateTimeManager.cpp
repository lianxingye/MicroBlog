//
//  DateTimeManager.cpp
//  MicroBlog
//
//  Created by tosoth on 6/27/15.
//
//

#include "DateTimeManager.h"

void DateTimeManage::refreshToCurrentTime()
{
    time_t currentTime;
    struct tm *localTime;
    
    time( &currentTime );                   // Get the current time
    localTime = localtime( &currentTime );  // Convert the current time to the local time
    
    Day    = localTime->tm_mday;
    Month  = localTime->tm_mon + 1;
    Year   = localTime->tm_year + 1900;
    Hour   = localTime->tm_hour;
    Min    = localTime->tm_min;
    Sec    = localTime->tm_sec;
}

int DateTimeManage::howManySecSince(time_t targetTm)
{
    time_t currentTime;
    struct tm *localTime;
    
    time( &currentTime );                   // Get the current time
    localTime = localtime( &currentTime );  // Convert the current time to the local time
    

    CCLOG("Diff %.f",difftime(currentTime, targetTm));
    return difftime(currentTime, targetTm);
}

time_t DateTimeManage::getCurrentTime()
{
    time_t currentTime;
    struct tm *localTime;
    
    time( &currentTime );                   // Get the current time
    localTime = localtime( &currentTime );  // Convert the current time to the local time
    
    int Day    = localTime->tm_mday;
    int Month  = localTime->tm_mon + 1;
    int Year   = localTime->tm_year + 1900;
    int Hour   = localTime->tm_hour;
    int Min    = localTime->tm_min;
    int Sec    = localTime->tm_sec;
    
    CCString* a = CCString::createWithFormat("%d 日, %d月 %d年 %d: %d: %d since January 1, 2000 in the current timezone", Day, Month, Year, Hour, Min,Sec);
    
    CCLOG("%s",a->getCString());
    
    return currentTime;

}