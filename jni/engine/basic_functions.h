/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_SYSTEM
#define H_SYSTEM
#include <SFML/Graphics.hpp>

/// NUMBERS
int random_int(int min, int max);
float random_float(float min, float max);

/// VECTORS
sf::Vector2f vector_normalize(const sf::Vector2f& source);
float vector_angle(const sf::Vector2f& Dir);
int vector_length(const sf::Vector2f& v);
void vector_rotate(sf::Vector2f &Center, sf::Vector2f *pPoint, float Rotation);

// CONVERSIONS
float angle_to_radians(float angle);
sf::Vector2f angle_to_direction(float angle);

/// DEBUG
void dbg_msg(const char *sys, const char *fmt, ...);

#endif
