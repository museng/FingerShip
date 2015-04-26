/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_GAME_CORE
#define H_GAME_CORE
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "collision.h"
#include "particle.h"
#include "player.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "../entities/entity.h"
#include "../screens/screen.h"
#include <list>

#define GAME_VERSION			"3.4"

#define SIZE_W 					10
#define SIZE_H 					16

#define TILE_SIZE 				64

#define TILES_MARGIN			3 // Margen superior no visible para el usuario
#define RTILES_MARGIN			TILE_SIZE*TILES_MARGIN

#define SIZE_WAY				2 // Tama�o del camino (1 + 2*2)

#define RSIZE_W					TILE_SIZE*SIZE_W
#define RSIZE_H					TILE_SIZE*SIZE_H

#define USER_ZONE_LIMIT_UP 		7
#define USER_ZONE_LIMIT_DOWN 	13

#define PIXELS_IN_METER 		3779.0f
#define MAX_METERS				15.0f

//#define DEBUG_MODE

enum
{
	TRANSITION_NONE = 0,

	TRANSITION_SLIDE_L,
	TRANSITION_SLIDE_R,
	TRANSITION_SLIDE_DIAGONAL_LU,
	TRANSITION_ROLL_ZOOM_OUT,
};

class CGameCore
{
	static CGameCore *m_pCoreInstance;

public:
	static CGameCore* getInstance()
	{
		if (!m_pCoreInstance)
		{
			m_pCoreInstance = new CGameCore();
			atexit(&destroyInstance);
		}

		return m_pCoreInstance;
	}

	~CGameCore();

	struct {
		bool m_LowGraphics;
		short m_Language;
	} m_Config;

	bool m_TurretKilled;
	bool m_MineKilled;
	bool m_EnemyShipKilled;
	bool m_AsteroidKilled;
	bool m_DronLaserKilled;
	bool m_SpacemanRescue;

	sf::Vector2f m_VirtualCameraPos;

	// TODO: Mirar de quitar esto
	void init();

	// Principal
	sf::RenderWindow* 	Window() { return m_pWindow; }
	CTextureManager* 	TextureManager() { return m_pTextureManager; }
	CSoundManager* 		SoundManager() { return m_pSoundManager; }
	CPlayer* 			Player() { return m_pPlayer; }
	class CCollision*	Collision() { return m_pCollision; }

	// Sistema
	sf::Font& getDefaultFont() { return *m_pDefFont; }
	void setScreen(int id);
	class CScreen* getScreen() { return m_pScreen; }
	int getLastScreenId() const { return m_LastScreenId; }
	int getCurrentScreenId() const { return m_CurrentScreenId; }
	void startTransitionTo(int id, int type);
	float getGameSpeed();
	bool hasFocus() const { return m_WinFocus; }
	bool isPaused() const { return m_Paused; }
	void setPaused(bool state);
	void setBackgroundColor(sf::Color color) { m_BackgroundColorTo = color; }

	// Tiempo
	float getFrameTime() const { return m_FrameTime; }
	sf::Time getGameTime() const { return m_GameClock.getElapsedTime(); }

protected:
	CGameCore();
	CGameCore(const CGameCore&);
	CGameCore& operator= (const CGameCore&);

private:
	static void destroyInstance()
	{
		delete m_pCoreInstance;
		m_pCoreInstance = 0x0;
	}

	sf::RenderWindow *m_pWindow;
	sf::Time m_GameTime;
	sf::Font *m_pDefFont;
	sf::Clock m_FrameTimeClock;
	sf::Clock m_GameClock;

	int m_TransitionType;
	sf::Clock m_TransitionTimer;
	bool m_TransitionRun;

	CPlayer *m_pPlayer;
	class CCollision *m_pCollision;
	CSoundManager *m_pSoundManager;
	CTextureManager *m_pTextureManager;
	class CScreen *m_pScreen;
	class CScreen *m_pPrevScreen;
	class CScreen *m_pNextScreen;
	int m_LastScreenId;
	int m_CurrentScreenId;

	bool m_Paused;
	bool m_WinFocus;
	float m_FrameTime;
	sf::Color m_BackgroundColor;
	sf::Color m_BackgroundColorTo;
};

#endif
