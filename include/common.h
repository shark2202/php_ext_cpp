#ifndef _INCLUDE_COMMON_H
#define _INCLUDE_COMMON_H

#include <stdio.h>

//增加一个调试的assert功能
#ifdef DEBUG
    #define ASSERT(condition,errMsg)\
    do{\
        if(!(condition)){\
            fprintf(stderr,"ASSERT failed! %s:%d in function %s(): %s\n",\
                __FILE__,__LINE__,__func__,errMsg);\
            abort();\
        }\
    }while(0);
#else 
    #define ASSERT(condition,errMsg) ((void)0)
#endif

#endif