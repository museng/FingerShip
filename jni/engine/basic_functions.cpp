/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "basic_functions.h"
#include <android/log.h>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>


int random_int(int min, int max)
{
	int range = max-min+1;
	return rand()%range + min;
}

float random_float(float min, float max)
{
    return min + (float)rand()/((float)RAND_MAX/(max-min));
}

sf::Vector2f vector_normalize(const sf::Vector2f& source)
{
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return sf::Vector2f(source.x / length, source.y / length);
    else
        return source;
}

float vector_angle(const sf::Vector2f& Dir)
{
	return atan2(Dir.x, Dir.y) * 180/M_PI;
}

int vector_length(const sf::Vector2f& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

void vector_rotate(sf::Vector2f &Center, sf::Vector2f *pPoint, float Rotation)
{
	int x = pPoint->x - Center.x;
	int y = pPoint->y - Center.y;
	pPoint->x = (int)(x * cosf(Rotation) - y * sinf(Rotation) + Center.x);
	pPoint->y = (int)(x * sinf(Rotation) + y * cosf(Rotation) + Center.y);
}

float angle_to_radians(float angle)
{
	return angle/360.0f*M_PI*2;
}

sf::Vector2f angle_to_direction(float angle)
{
	angle = angle/180;
	return sf::Vector2f(cosf(angle), sinf(angle));
}

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
