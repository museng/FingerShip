/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "basic_functions.h"
#include <android/log.h>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>


/*
 * Functions original in Teeworlds Source
 */
#ifdef __cplusplus
extern "C" {
#endif

void dbg_msg(const char *sys, const char *fmt, ...)
{
	va_list args;
	char str[1024*4];
	char *msg;
	int i, len;

	snprintf(str, sizeof(str), "[%08x][%s]: ", (int)time(0), sys);
	len = strlen(str);
	msg = (char *)str + len;

	va_start(args, fmt);
	vsnprintf(msg, sizeof(str)-len, fmt, args);
	va_end(args);

	__android_log_print(ANDROID_LOG_DEBUG, sys, "%s", msg);
}

#ifdef __cplusplus
}
#endif
