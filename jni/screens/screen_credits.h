/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_SCREEN_CREDITS
#define H_SCREEN_CREDITS
#include <SFML/Graphics.hpp>
#include "screen.h"

class CScreenCredits : public CScreen
{
public:
	CScreenCredits(int camW, int camH);
	~CScreenCredits();

	void tick();
	sf::View& updateCamera();

protected:
	sf::Clock m_Timer;
	int m_StrIndex;
	bool m_End;
	float m_TextZoom;
};

#endif
