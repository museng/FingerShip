/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_SYSTEM
#define H_SYSTEM
#include <SFML/Graphics.hpp>
#include <cmath>

inline int random_int(int min, int max)
{
	int range = max-min+1;
	return rand()%range + min;
}

inline float random_float(float min, float max)
{
    return min + (float)rand()/((float)RAND_MAX/(max-min));
}

inline sf::Vector2f vector_normalize(const sf::Vector2f& source)
{
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return sf::Vector2f(source.x / length, source.y / length);
    else
        return source;
}

inline float vector_angle(const sf::Vector2f& Dir)
{
	return atan2(Dir.x, Dir.y) * 180/M_PI;
}

inline int vector_length(const sf::Vector2f& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

inline void vector_rotate(const sf::Vector2f &Center, sf::Vector2f *pPoint, float RotationRadians)
{
	int x = pPoint->x - Center.x;
	int y = pPoint->y - Center.y;
	pPoint->x = (int)(x * cosf(RotationRadians) - y * sinf(RotationRadians) + Center.x);
	pPoint->y = (int)(x * sinf(RotationRadians) + y * cosf(RotationRadians) + Center.y);
}

inline float vector_dot(const sf::Vector2f &vectorA, const sf::Vector2f &vectorB)
{
	return vectorA.x*vectorB.x + vectorA.y*vectorB.y;
}

inline float angle_to_radians(float angle)
{
	return angle/360.0f*M_PI*2;
}

inline sf::Vector2f angle_to_direction(float angle)
{
	angle = angle/180;
	return sf::Vector2f(cosf(angle), sinf(angle));
}


/// DEBUG
#ifdef __cplusplus
extern "C" {
#endif

	void get_internal_file_path(const char *filePath, char *finalPath, size_t size);

	void dbg_msg(const char *sys, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
