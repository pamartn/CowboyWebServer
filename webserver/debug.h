#ifndef __DEBUG_H__
# define __DEBUG_H__


//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)    /* Don't do anything in release builds */
#endif

#endif
