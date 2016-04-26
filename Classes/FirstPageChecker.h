//
//  FirstPage.hpp
//  MicroBlog
//
//  Created by tosoth on 4/14/16.
//
//

#ifndef FirstPage_h
#define FirstPage_h

#include <stdio.h>
#include <cocos2d.h>

#include "HelloWorldScene.h"

USING_NS_CC;

class FirstPageChecker
{
public:
    int firstShowtimes;
    void getCurrentTime();
    void checkTime();
    
    static CCScene* checkifFirstPageNeeded();
};
#endif /* FirstPage_hpp */
