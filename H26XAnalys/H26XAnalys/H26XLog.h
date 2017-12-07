//
//  H26XLog.h
//  H26XAnalys
//
//  Created by lunli on 2017/12/7.
//  Copyright © 2017年 lunli. All rights reserved.
//

#ifndef H26XLog_h
#define H26XLog_h

#include <stdio.h>
#include <time.h>

#if 1

#define eprintf(...)  do { fprintf(stderr, "%s|ERR|%s| (%s, line %d): ",  __TIMESTAMP__, __func__, __FILE__, __LINE__); \
                           fprintf(stderr, __VA_ARGS__);           \
                           fprintf(stderr, "\n");} while(0)

#define dprintf(...)  do { fprintf(stderr, "%s|INFO|%s| (%s, line %d): ", __TIMESTAMP__, __func__, __FILE__, __LINE__); \
                           fprintf(stderr, __VA_ARGS__);           \
                           fprintf(stderr, "\n");} while(0)
#else

#define eprintf(...)
#define dprintf(...)

#endif

#endif /* H26XLog_h */
