//
//  FramDataSet.h
//  MicroBlog
//
//  Created by tosoth on 6/7/15.
//
//

#ifndef __MicroBlog__FramDataSet__
#define __MicroBlog__FramDataSet__

#include <stdio.h>
#include <cocos2d.h>

USING_NS_CC;

class FramDataSet
{
public:
    CCArray* allItems;
    int iItemCount;
    struct tm *setTime;
    
};

#endif /* defined(__MicroBlog__FramDataSet__) */
