/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_SCREEN_OPTIONS
#define H_SCREEN_OPTIONS
#include <SFML/Graphics.hpp>
#include "screen.h"

class CScreenOptions : public CScreen
{
public:
	CScreenOptions(int camW, int camH);
	~CScreenOptions();

	void tick();
	sf::View& updateCamera();
};

#endif
