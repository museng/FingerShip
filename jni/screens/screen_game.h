/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_SCREEN_GAME
#define H_SCREEN_GAME
#include <SFML/Graphics.hpp>
#include "screen.h"

class CScreenGame : public CScreen
{
public:
	enum
	{
		STARTED=0,
		GAME_OVER,
		FINISHED,
		PAUSED,
	};

	CScreenGame(int camW, int camH);
	~CScreenGame();

	virtual void onSignal(int signal);
	void tick();
	sf::View& updateCamera();

	// TODO: Mirar de quitar esto de aqui
	void createPointsEffect();

protected:
	float m_TextZoom;
	int m_GameState;
	int m_LastMapZone;
	bool m_End;

	bool m_DoFinish;
	sf::Clock m_FinishTimer;

	bool m_CameraQuake;
	sf::Clock m_QuakeTimer;

	bool m_DoRotateScreen;
	sf::Clock m_RotateScreenTimer;

	sf::Text m_HudTextScore;

private:
	void renderHUD();
	void resetGame();
	void startGame();
	void endGame();

	void setGameState(int state);
};

#endif
