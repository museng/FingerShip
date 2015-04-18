/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_SCREEN_GAME_FINISHED
#define H_SCREEN_GAME_FINISHED
#include <SFML/Graphics.hpp>
#include "screen.h"

class CScreenGameFinished : public CScreen
{
public:
	CScreenGameFinished(int camW, int camH);
	~CScreenGameFinished();

	void tick();
	sf::View& updateCamera();

protected:
	sf::Clock m_FireworksTimer;
	sf::Clock m_Timer;
};

#endif
