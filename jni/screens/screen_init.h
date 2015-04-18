/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_SCREEN_INIT
#define H_SCREEN_INIT
#include <SFML/Graphics.hpp>
#include "screen.h"

class CScreenInit : public CScreen
{
public:
	CScreenInit(int camW, int camH);
	~CScreenInit();

	void tick();
	sf::View& updateCamera();
};

#endif
