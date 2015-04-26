/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "game_core.h"
#include "basic_functions.h"
//#include <jni.h>
//#include <android/native_activity.h>
#include "effects.h"
#include "locale.h"
#include "../screens/screen_init.h"
#include "../screens/screen_options.h"
#include "../screens/screen_game.h"
#include "../screens/screen_game_finished.h"
#include "../screens/screen_credits.h"

#include <android/log.h>

CGameCore* CGameCore::m_pCoreInstance = nullptr;
CGameCore::CGameCore()
{
	m_pWindow = nullptr;
	m_pPlayer = nullptr;
	m_pScreen = nullptr;
	m_pCollision = nullptr;
	m_pDefFont = nullptr;
	m_pSoundManager = nullptr;
	m_pTextureManager = nullptr;

	m_pNextScreen = nullptr;
	m_pPrevScreen = nullptr;
	m_TransitionType = TRANSITION_NONE;
	m_TransitionTimer.restart();
	m_TransitionRun = false;

	m_TurretKilled = false;
	m_MineKilled = false;
	m_EnemyShipKilled = false;
	m_AsteroidKilled = false;
	m_DronLaserKilled = false;
	m_SpacemanRescue = false;

	m_Config.m_LowGraphics = false;
	m_Config.m_Language = LANGUAGE_ES;
	m_WinFocus = true;
	m_Paused = false;
	m_FrameTime = 0.0f;

	m_LastScreenId = m_CurrentScreenId = CScreen::INIT;
	m_BackgroundColorTo = m_BackgroundColor = sf::Color::Black;
}
CGameCore::~CGameCore()
{
	if (m_pDefFont) delete m_pDefFont;
	if (m_pScreen) delete m_pScreen;
	if (m_pCollision) delete m_pCollision;
	if (m_pPlayer) delete m_pPlayer;
	if (m_pSoundManager) delete m_pSoundManager;
	if (m_pTextureManager) delete m_pTextureManager;
	if (m_pWindow) delete m_pWindow;
}

void CGameCore::init()
{
	// Load Window
	m_pWindow = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "", sf::Style::Fullscreen);
	if (!m_pWindow)
		return;
	m_pWindow->setFramerateLimit(60.0f);
	//

	// Load Assets
	m_pDefFont = new sf::Font();
	if (!m_pDefFont || !m_pDefFont->loadFromFile("fonts/Pixel Musketeer.ttf"))
		return;

	m_pTextureManager = new CTextureManager();
	if (!m_pTextureManager || !m_pTextureManager->load())
		return;

	m_pSoundManager = new CSoundManager();
	if (!m_pSoundManager || !m_pSoundManager->load())
		return;
	//

    //ANativeWindow* winNat = (ANativeWindow*)m_pWindow->getSystemHandle();
    //m_pGooglePlayGames = new CGooglePlayGames((ANativeActivity*)m_pWindow->getNativeActivity());

	// Initialize Envirionment
	m_pCollision = new CCollision();
	m_pPlayer = new CPlayer();
	if (!m_pCollision || !m_pPlayer)
		return;
	//

	// Set Initial Screen
	m_CurrentScreenId = CScreen::INIT;
    setScreen(CScreen::INIT);

    // Main Loop
    bool m_Running = true;
    while (m_pWindow->isOpen())
    {
    	m_FrameTime = m_FrameTimeClock.restart().asSeconds();

    	SoundManager()->checkPlayedSounds(); // Clean Sounds Buffer

    	// Events
        sf::Event event;
        while (m_pWindow->pollEvent(event))
        {
        	switch (event.type)
            {
            	case sf::Event::Closed:
				{
					m_Running = false;
					m_pWindow->close();
				} break;
				case sf::Event::GainedFocus:
				{
					m_WinFocus = true;
					if (m_CurrentScreenId != CScreen::GAME)
						setPaused(false);
				} break;
				case sf::Event::LostFocus:
				{
					m_WinFocus = false;
					setPaused(true);
				} break;
				case sf::Event::Resized:
				{
					m_pScreen->setSize(RSIZE_W, RSIZE_H);
				} break;
            }
        }
        //

        if (hasFocus())
        {
			// Draw Game
        	if (m_BackgroundColor.r < m_BackgroundColorTo.r) m_BackgroundColor.r++;
        	else if (m_BackgroundColor.r > m_BackgroundColorTo.r) m_BackgroundColor.r--;
        	if (m_BackgroundColor.g < m_BackgroundColorTo.g) m_BackgroundColor.g++;
        	else if (m_BackgroundColor.g > m_BackgroundColorTo.g) m_BackgroundColor.g--;
        	if (m_BackgroundColor.b < m_BackgroundColorTo.b) m_BackgroundColor.b++;
        	else if (m_BackgroundColor.b > m_BackgroundColorTo.b) m_BackgroundColor.b--;
        	if (m_BackgroundColor.a < m_BackgroundColorTo.a) m_BackgroundColor.a++;
        	else if (m_BackgroundColor.a > m_BackgroundColorTo.a) m_BackgroundColor.a--;
			m_pWindow->clear(m_BackgroundColor);

			// Render Screen & Transitions
			if (m_TransitionRun)
			{
				if (m_TransitionType == TRANSITION_SLIDE_L)
				{
					m_pNextScreen->Camera()->move(30.0f, 0.0f);
					m_pPrevScreen->Camera()->move(30.0f, 0.0f);
					sf::Vector2f nsp = m_pNextScreen->Camera()->getCenter();
					sf::FloatRect sp = m_pPrevScreen->Camera()->getViewport();


					m_pScreen = m_pPrevScreen;
					m_pWindow->setView(m_pScreen->updateCamera());
					m_pScreen->tick();

					m_pScreen = m_pNextScreen;
					m_pWindow->setView(m_pNextScreen->updateCamera());
					m_pScreen->tick();

					sf::Vector2f camSize = m_pScreen->Camera()->getSize();
					if (nsp.x-camSize.x/2 > 0.0f)
					{
						delete m_pPrevScreen;
						m_pScreen = m_pNextScreen;
						m_pPrevScreen = nullptr;
						m_pNextScreen = nullptr;
						m_TransitionRun = false;

						m_pScreen->Camera()->setCenter(camSize.x/2, camSize.y/2);
						m_pWindow->setView(m_pScreen->updateCamera());
						m_pScreen->m_InputActive = true;
					}
				}
				else if (m_TransitionType == TRANSITION_SLIDE_R)
				{
					m_pNextScreen->Camera()->move(-30.0f, 0.0f);
					m_pPrevScreen->Camera()->move(-30.0f, 0.0f);
					sf::Vector2f nsp = m_pNextScreen->Camera()->getCenter();
					sf::FloatRect sp = m_pPrevScreen->Camera()->getViewport();


					m_pScreen = m_pPrevScreen;
					m_pWindow->setView(m_pScreen->updateCamera());
					m_pScreen->tick();

					m_pScreen = m_pNextScreen;
					m_pWindow->setView(m_pNextScreen->updateCamera());
					m_pScreen->tick();

					sf::Vector2f camSize = m_pScreen->Camera()->getSize();
					if (nsp.x-camSize.x/2 < 0.0f)
					{
						delete m_pPrevScreen;
						m_pScreen = m_pNextScreen;
						m_pPrevScreen = nullptr;
						m_pNextScreen = nullptr;
						m_TransitionRun = false;

						m_pScreen->Camera()->setCenter(camSize.x/2, camSize.y/2);
						m_pWindow->setView(m_pScreen->updateCamera());
						m_pScreen->m_InputActive = true;
					}
				}
				else if (m_TransitionType == TRANSITION_SLIDE_DIAGONAL_LU)
				{
					sf::Vector2f camSize = m_pPrevScreen->Camera()->getSize();
					sf::Vector2f direction = vector_normalize(camSize);

					m_pNextScreen->Camera()->move(30.0f*direction.x, 30.0f*direction.y);
					m_pPrevScreen->Camera()->move(30.0f*direction.x, 30.0f*direction.y);
					sf::Vector2f nsp = m_pNextScreen->Camera()->getCenter();
					sf::FloatRect sp = m_pPrevScreen->Camera()->getViewport();


					m_pScreen = m_pPrevScreen;
					m_pWindow->setView(m_pScreen->updateCamera());
					m_pScreen->tick();

					m_pScreen = m_pNextScreen;
					m_pWindow->setView(m_pNextScreen->updateCamera());
					m_pScreen->tick();

					if (nsp.x-camSize.x/2 > 0.0f)
					{
						delete m_pPrevScreen;
						m_pScreen = m_pNextScreen;
						m_pNextScreen = nullptr;
						m_pPrevScreen = nullptr;
						m_TransitionRun = false;

						m_pScreen->Camera()->setCenter(camSize.x/2, camSize.y/2);
						m_pWindow->setView(m_pScreen->updateCamera());
						m_pScreen->m_InputActive = true;
					}
				}
				else if (m_TransitionType == TRANSITION_ROLL_ZOOM_OUT)
				{
					sf::Vector2f camSize = m_pScreen->Camera()->getSize();

					m_pScreen = m_pNextScreen;
					m_pScreen->Camera()->reset(sf::FloatRect(0, 0, RSIZE_W, RSIZE_H));
					m_pWindow->setView(m_pScreen->updateCamera());
					m_pScreen->tick();

					m_pScreen = m_pPrevScreen;
					m_pScreen->Camera()->zoom(1.15f);
					m_pScreen->Camera()->rotate(10.0f);
					m_pWindow->setView(m_pScreen->updateCamera());
					m_pScreen->tick();

					if (camSize.x > RSIZE_W*8)
					{
						delete m_pPrevScreen;
						m_pScreen = m_pNextScreen;
						m_pNextScreen = nullptr;
						m_pPrevScreen = nullptr;
						m_TransitionRun = false;

						m_pScreen->Camera()->zoom(1.0f);
						m_pScreen->Camera()->reset(sf::FloatRect(0, 0, RSIZE_W, RSIZE_H));
						m_pWindow->setView(m_pScreen->updateCamera());
						m_pScreen->m_InputActive = true;
					}
				}
			}
			else
			{
				m_pWindow->setView(m_pScreen->updateCamera());
				m_pScreen->tick();
			}

			m_pWindow->display();
        }
    }
}

void CGameCore::setScreen(int id)
{
	if (id < 0 || id >= CScreen::MAX_SCREENS)
		return;

	if (m_pScreen)
		delete m_pScreen;

	if (id == CScreen::INIT)
		m_pScreen = new CScreenInit(RSIZE_W, RSIZE_H);
	else if (id == CScreen::OPTIONS)
		m_pScreen = new CScreenOptions(RSIZE_W, RSIZE_H);
	else if (id == CScreen::GAME)
		m_pScreen = new CScreenGame(RSIZE_W, RSIZE_H);
	else if (id == CScreen::GAME_FINISH)
		m_pScreen = new CScreenGameFinished(RSIZE_W, RSIZE_H);
	else if (id == CScreen::CREDITS)
		m_pScreen = new CScreenCredits(RSIZE_W, RSIZE_H);

	m_LastScreenId = m_CurrentScreenId;
	m_CurrentScreenId = id;
}

void CGameCore::startTransitionTo(int id, int type)
{
	if (id < 0 || id >= CScreen::MAX_SCREENS)
		return;

	if (m_pNextScreen)
		delete m_pNextScreen;

	m_pPrevScreen = m_pScreen;

	if (id == CScreen::INIT)
		m_pNextScreen = new CScreenInit(RSIZE_W, RSIZE_H);
	else if (id == CScreen::OPTIONS)
		m_pNextScreen = new CScreenOptions(RSIZE_W, RSIZE_H);
	else if (id == CScreen::GAME)
		m_pNextScreen = new CScreenGame(RSIZE_W, RSIZE_H);
	else if (id == CScreen::GAME_FINISH)
		m_pNextScreen = new CScreenGameFinished(RSIZE_W, RSIZE_H);
	else if (id == CScreen::CREDITS)
		m_pNextScreen = new CScreenCredits(RSIZE_W, RSIZE_H);

	m_TransitionType = type;
	m_TransitionTimer.restart();
	m_TransitionRun = true;

	if (type == TRANSITION_SLIDE_L)
		m_pNextScreen->Camera()->move(-RSIZE_W, 0);
	else if (type == TRANSITION_SLIDE_R)
		m_pNextScreen->Camera()->move(RSIZE_W, 0);
	else if (TRANSITION_SLIDE_DIAGONAL_LU)
		m_pNextScreen->Camera()->move(-RSIZE_W, -RSIZE_H);

	m_pPrevScreen->m_InputActive = false;
	m_pNextScreen->m_InputActive = false;

	m_LastScreenId = m_CurrentScreenId;
	m_CurrentScreenId = id;
}

void CGameCore::setPaused(bool state)
{
	m_Paused = state;

	if (m_Paused) m_pSoundManager->pauseAll();
	else m_pSoundManager->resumeAll();
}

float CGameCore::getGameSpeed()
{
	if (m_Paused)
		return 0.0f;
	else if (Collision()->Map())
		return Collision()->Map()->m_Speed;

	return 5.0f;
}
