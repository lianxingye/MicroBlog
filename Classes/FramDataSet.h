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
    // all the view items
    CCArray* allItems;
    
    // numbers of items
    int iItemCount;
    
    // time struct
    struct tm *setTime;
    
    // type of the frame
    int frameType;
    
    // this decide the sequence, the bigger the num is, more often yout will see it
    int constrants;
};

#endif /* defined(__MicroBlog__FramDataSet__) */
