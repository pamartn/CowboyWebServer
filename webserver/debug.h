#ifndef __DEBUG_H__
# define __DEBUG_H__


/*
	Debug header to define debug macro.
	Found on http://stackoverflow.com/questions/1941307/c-debug-print-macros
*/

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)    /* Don't do anything in release builds */
#endif

#endif
