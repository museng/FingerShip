/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "screen_game.h"
#include "../engine/game_core.h"
#include "../engine/locale.h"
#include "../engine/effects.h"
#include "../entities/powerup.h"
#include "../entities/projectile.h"

CScreenGame::CScreenGame(int camW, int camH)
: CScreen(camW, camH)
{
	startGame();
}
CScreenGame::~CScreenGame()
{
	Core()->Collision()->reset();
	Core()->Player()->destroyCharacter(false);
}

void CScreenGame::resetGame()
{
	clearEntities();
	clearParticles();

	Core()->setPaused(false);

	sf::Vector2f camSize = Camera()->getSize();
	Camera()->setCenter(camSize.x/2, camSize.y/2);
	Core()->Collision()->init(new CMap());
	Core()->Player()->destroyCharacter(false);
	Core()->Player()->m_pCharacter = new CCharacter();
	Core()->Player()->clearPoints();
	m_GameState = STARTED;
	m_CameraQuake = false;
	m_DoRotateScreen = false;
	m_DoFinish = false;
	m_End = false;

	// Started Stars...
	for (int i=0; i<RSIZE_W; i++)
	{
		for (int e=0; e<RSIZE_H; e+=TILE_SIZE/2)
		{
			if (random_int(0, 1000) == 5)
			{
				CParticleStar *pStar = new CParticleStar();
				pStar->m_Pos = sf::Vector2f(i, e);
				addParticle(pStar);
			}
		}
	}
}

void CScreenGame::startGame()
{
	Core()->SoundManager()->play(CSoundManager::SOUND_MENU_SELECT);
	resetGame();
	Core()->SoundManager()->playBackgroundMusic(CSoundManager::MUSIC_ZONE_A);
}

void CScreenGame::endGame()
{
	Core()->SoundManager()->stopBackgroundMusic();
	Core()->SoundManager()->play(CSoundManager::SOUND_GAME_OVER);
	setGameState(GAME_OVER);
	if (Core()->Collision()->Map()->getZone() != CMap::ZONE_D)
		Core()->Collision()->Map()->m_Speed = 0;
	Core()->Collision()->Map()->setStatus(CMap::END);

	Camera()->setRotation(0.0f);
	m_DoRotateScreen = false;
	m_InputActive = true;

	m_CameraQuake = true;
	m_QuakeTimer.restart();
	m_TextZoomNoob = 5.0f;
}


void CScreenGame::onSignal(int signal)
{
	if (signal == SIGNAL_GAME_FINISH)
	{
		m_End = true;
		Core()->Player()->destroyCharacter(true);
	}
	else if (signal == SIGNAL_BOSS_SPACESTATION_DETROYED)
	{
		m_FinishTimer.restart();
		m_pCore->setBackgroundColor(sf::Color::White);
		Core()->SoundManager()->stopBackgroundMusic();
		Core()->SoundManager()->play(CSoundManager::SOUND_WINNER);
		m_DoFinish = true;
		m_TextZoomNoob = 5.0f;

		setGameState(FINISHED);
	}
}

void CScreenGame::tick()
{
	// Pause Control
	static int prevGameState = -1;
	if (Core()->isPaused() && m_GameState != PAUSED)
	{
		m_TextZoomNoob = 0.0f;
		prevGameState = m_GameState;
		m_GameState = PAUSED;
	}
	else if (!Core()->isPaused() && m_GameState == PAUSED)
		m_GameState = prevGameState;


	if (m_GameState == FINISHED)
	{
		if (!m_CameraQuake && m_FinishTimer.getElapsedTime().asSeconds() > 2.3f && m_FinishTimer.getElapsedTime().asSeconds() < 2.5f)
		{
			m_CameraQuake = true;
			m_QuakeTimer.restart();
		}
		if (m_FinishTimer.getElapsedTime().asSeconds() > 3.1f)
		{
			Core()->SoundManager()->playBackgroundMusic(CSoundManager::MUSIC_WINNER);
			Core()->setScreen(CScreen::CREDITS);
			return;
		}
	}
	else if (m_GameState == STARTED)
	{
		// Check Player Status
		if (Core()->Player()->m_pCharacter)
		{
			CEntity *pEntColl = Core()->Collision()->isInEntity(Core()->Player()->m_pCharacter->getCollChar().getGlobalBounds());
			CTile *pTile = Core()->Collision()->intersectLineTileSolid(Core()->Player()->m_pCharacter->getPrevPosition(), Core()->Player()->m_pCharacter->getPosition());
			if (pTile || pEntColl)
			{
				if (pEntColl)
				{
					if (pEntColl->getType() == CEntity::SPACEMAN)
					{
						Core()->m_SpacemanRescue = true;
						Core()->Player()->addPoints(750);
						Core()->Player()->m_pCharacter->m_OverHeat = 0.0f;
						Core()->Player()->m_pCharacter->m_OverHeating = false;
						CEffects::createStarExplosion(Core()->Player()->m_pCharacter->getPosition());
						Core()->SoundManager()->play(CSoundManager::SOUND_SPACEMAN);

						// Reset Entities
						std::list<CEntity*>::iterator it = getEntities()->begin();
						while (it != getEntities()->end())
						{
							if ((*it)->getType() != CEntity::SPACEMAN)
							{
								if ((*it)->getType() == CEntity::PROJECTILE)
								{
									CProjectile *pProjectile = static_cast<CProjectile*>((*it));
									if (pProjectile->m_Owner != CEntity::CHARACTER)
										(*it)->destroy();
								}
								else if ((*it)->getType() != CEntity::SPACESTATION)
									(*it)->destroy();
							}

							++it;
						}

						pEntColl->setToDelete();
					}
					else if (pEntColl->getType() == CEntity::POWERUP)
					{
						CPowerup *pPowerup = static_cast<CPowerup*>(pEntColl);

						if (pPowerup->getType() == CPowerup::SHIELD)
							Core()->Player()->m_pCharacter->giveShield();
						else if (pPowerup->getType() == CPowerup::BAD_BOLT)
						{
							if (!m_DoRotateScreen)
							{
								Core()->SoundManager()->play(CSoundManager::SOUND_POWERUP_BAD_BOLT_A);
								m_DoRotateScreen = true;
								m_InputActive = false;
							}

							m_RotateScreenTimer.restart();
						}

						Core()->SoundManager()->play(CSoundManager::SOUND_POWERUP);
						CEffects::createPowerupPickup(pEntColl->getPosition(), pPowerup->getType());
						pEntColl->setToDelete();
					}
					else
					{
						if (Core()->Player()->m_pCharacter->hasShield())
							Core()->Player()->m_pCharacter->dropShield();
						else
							Core()->Player()->destroyCharacter(true);

						if (pEntColl->getType() != CEntity::CUBE && pEntColl->getType() != CEntity::SPACESTATION)
							pEntColl->destroy();
					}
				}
				else
					Core()->Player()->destroyCharacter(true);
			}
		} else if (m_GameState != GAME_OVER && !m_End)
			endGame();
	}

	// Render Stuff
	CScreen::renderBack();

	m_LastMapZone = Core()->Collision()->Map()->getZone();
	Core()->Collision()->Map()->render();
	if (Core()->Player()->m_pCharacter)
		Core()->Player()->m_pCharacter->tick();

	CScreen::renderFront();

	renderHUD();
}

sf::View& CScreenGame::updateCamera()
{
	static bool soundPlayed = false;

	if (m_CameraQuake)
	{
		if (m_QuakeTimer.getElapsedTime().asMilliseconds() > 220)
		{
			m_CameraQuake = false;
			sf::Vector2f camSize = Camera()->getSize();
			Camera()->setCenter(camSize.x/2, camSize.y/2);
		}
		else
			Camera()->move(random_float(-3.5f, 3.5f), random_float(-3.5f, 3.5f));
	}

	if (m_DoRotateScreen)
	{
		m_InputActive = false;
		if (m_RotateScreenTimer.getElapsedTime().asSeconds() < 4.0f)
		{
			if (Camera()->getRotation() < 180.0f)
				Camera()->rotate(10.0f);
			else
			{
				Camera()->setRotation(180.0f);
				m_InputActive = true;
				soundPlayed = false;
			}
		} else
		{
			if (!soundPlayed)
			{
				Core()->SoundManager()->play(CSoundManager::SOUND_POWERUP_BAD_BOLT_B);
				soundPlayed = true;
			}

			if (Camera()->getRotation() > 0.0f)
				Camera()->rotate(-10.0f);
			else
			{
				Camera()->setRotation(0.0f);
				m_DoRotateScreen = false;
				m_InputActive = true;
			}
		}
	}

	return *Camera();
}

void CScreenGame::renderHUD()
{
	sf::Text text;
	text.setFont(Core()->getDefaultFont());

	char buff[128];

	// Puntuacion
	snprintf(buff, sizeof(buff), "%09d", Core()->Player()->getPoints());
	m_HudTextScore.setFont(Core()->getDefaultFont());
	m_HudTextScore.setString(buff);
	m_HudTextScore.setCharacterSize(25);
	m_HudTextScore.setPosition(sf::Vector2f(10.0f, 10.0f));
	m_HudTextScore.setColor(Core()->Player()->getPoints()<0?sf::Color::Red:sf::Color::Green);
	m_HudTextScore.setStyle(sf::Text::Bold);
	Core()->Window()->draw(m_HudTextScore);

	// Debug
	#ifdef DEBUG_MODE
		snprintf(buff, sizeof(buff), "Entidades: %d", m_vpEntities.size());
		text.setString(buff);
		text.setPosition(sf::Vector2f(5, 110));
		text.setColor(sf::Color::Yellow);
		Core()->Window()->draw(text);

		snprintf(buff, sizeof(buff), "Particulas: %d", m_vpParticles.size());
		text.setString(buff);
		text.setPosition(sf::Vector2f(5, 140));
		text.setColor(sf::Color::Yellow);
		Core()->Window()->draw(text);

		snprintf(buff, sizeof(buff), "Sonidos: %d", Core()->SoundManager()->getNumPlayingSound());
		text.setString(buff);
		text.setPosition(sf::Vector2f(5, 170));
		text.setColor(sf::Color::Yellow);
		Core()->Window()->draw(text);

		if (Core()->Collision()->Map())
		{
			snprintf(buff, sizeof(buff), "Velocidad: %.2f", Core()->getGameSpeed());
			text.setString(buff);
			text.setPosition(sf::Vector2f(5, 200));
			text.setColor(sf::Color::Yellow);
			Core()->Window()->draw(text);
		}
	#endif

	// Metros
	float meters =  MAX_METERS - Core()->Collision()->Map()->m_OffsetY/PIXELS_IN_METER;
	if (meters < 0.0f)
	{
		std::list<CEntity*> vBoss = getEntitiesByType(CEntity::SPACESTATION);
		if (vBoss.size() == 1)
		{
			std::list<CEntity*>::iterator itBoss = vBoss.begin();
			CBossSpaceStation *pBoss = static_cast<CBossSpaceStation*>((*itBoss));
			snprintf(buff, sizeof(buff), "[%s %d] %s", CLocale::getString(RESOURCE_STR_VIDA), pBoss->m_Health, CLocale::getString(RESOURCE_STR_JEFE_FINAL));
			text.setColor(sf::Color(random_int(128,255), 0, 0, 255));
		}
		else // TODO: Dudo que sea realmente necesario esto...
			buff[0] = 0;
	}
	else
	{
		snprintf(buff, sizeof(buff), "%s %.2f %s", CLocale::getString(RESOURCE_STR_QUEDAN), meters, CLocale::getString(RESOURCE_STR_METROS));
		text.setColor(sf::Color::Cyan);
	}
	text.setString(buff);
	text.setPosition(sf::Vector2f(RSIZE_W-text.getLocalBounds().width-10.0f, 10.0f));
	Core()->Window()->draw(text);

	// Soobrecalentamiento
	if (Core()->Player()->m_pCharacter)
	{
		const int maxBarSize = 200;
		sf::RectangleShape rectShapeBorder(sf::Vector2f(maxBarSize, 15.0f));
		rectShapeBorder.setPosition(sf::Vector2f(RSIZE_W-(maxBarSize+10), 45.0f));
		rectShapeBorder.setFillColor(sf::Color::Transparent);
		rectShapeBorder.setOutlineThickness(1.0f);
		rectShapeBorder.setOutlineColor(sf::Color::White);
		Core()->Window()->draw(rectShapeBorder);

		sf::RectangleShape rectShape(sf::Vector2f(Core()->Player()->m_pCharacter->m_OverHeat * maxBarSize * 0.01f, 15.0f));
		rectShape.setPosition(sf::Vector2f(RSIZE_W-(maxBarSize+10), 45.0f));
		rectShape.setFillColor(sf::Color::Red);
		Core()->Window()->draw(rectShape);

		if (Core()->Player()->m_pCharacter->m_OverHeating)
		{
			strncpy(buff, CLocale::getString(RESOURCE_STR_ARMA_SOBRECALENTADA), sizeof(buff));
			text.setString(buff);
			text.setPosition(sf::Vector2f(RSIZE_W-text.getLocalBounds().width-10.0f, 70.0f));
			text.setColor(sf::Color::Red);
			Core()->Window()->draw(text);
		}
	}

	if ((Core()->Collision()->Map()->getZone() != CMap::ZONE_D || Core()->Collision()->Map()->getZone() == CMap::ZONE_D && Core()->isPaused()) && (m_GameState == STARTED || m_GameState == PAUSED))
	{
		sf::RectangleShape btnPause(sf::Vector2f(64, 64));
		btnPause.setPosition(sf::Vector2f(RSIZE_W - btnPause.getLocalBounds().width, RSIZE_H - btnPause.getLocalBounds().height - 50.0f));
		btnPause.setTexture(Core()->TextureManager()->get((m_GameState==PAUSED)?CTextureManager::TEXTURE_RESUME:CTextureManager::TEXTURE_PAUSE));
		btnPause.setFillColor(sf::Color(255, 255, 255, 185));
		Core()->Window()->draw(btnPause);

		// Actions
		static bool tap = false;
		if (m_InputActive && sf::Touch::isDown(0) && !tap)
		{
			sf::Vector2f convCoords = Core()->Window()->mapPixelToCoords(sf::Touch::getPosition(0));
			if (btnPause.getGlobalBounds().contains(convCoords))
			{
				Core()->setPaused(!Core()->isPaused());
				Core()->SoundManager()->play(CSoundManager::SOUND_MENU_SELECT_B);
			}

			tap = true;
		} else if (!sf::Touch::isDown(0))
			tap = false;
	}

	if (m_GameState == GAME_OVER)
	{
		// Efecto Zoom out
		m_TextZoomNoob -= 0.25f;
		if (m_TextZoomNoob < 1.0f) m_TextZoomNoob = 1.0f;

		float ttp = USER_ZONE_LIMIT_DOWN - USER_ZONE_LIMIT_UP;
		ttp = USER_ZONE_LIMIT_UP + ttp/2;
		text.setString("NOOB!");
		text.setCharacterSize(162.0f);
		text.setColor(sf::Color::Red);
		text.setPosition(sf::Vector2f(RSIZE_W/2, ttp*TILE_SIZE - text.getLocalBounds().height/2));
		text.setStyle(sf::Text::Bold);
		text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
		text.setScale(m_TextZoomNoob, m_TextZoomNoob);
		Core()->Window()->draw(text);

		text.setScale(1.0f, 1.0f);
		text.setOrigin(0.0f, 0.0f);

		/*sf::RectangleShape areaButtons(sf::Vector2f(RSIZE_W, TILE_SIZE*1.5f));
		areaButtons.setPosition(sf::Vector2f(0, RSIZE_H - areaButtons.getLocalBounds().height - 70.0f));
		areaButtons.setFillColor(sf::Color(255, 255, 255, 128));
		Core()->Window()->draw(areaButtons);*/

		sf::RectangleShape btnExit(sf::Vector2f(TILE_SIZE*3, TILE_SIZE));
		btnExit.setOrigin(sf::Vector2f(0, btnExit.getLocalBounds().height/2));
		btnExit.setPosition(sf::Vector2f(RSIZE_W - btnExit.getLocalBounds().width, RSIZE_H - btnExit.getLocalBounds().height - 50.0f));
		btnExit.setFillColor(sf::Color(255, 54, 54, 255));
		btnExit.setOutlineThickness(2.0f);
		btnExit.setOutlineColor(sf::Color(127, 27, 27, 255));
		Core()->Window()->draw(btnExit);

		text.setString(CLocale::getString(RESOURCE_STR_SALIR));
		text.setColor(sf::Color::White);
		text.setCharacterSize(22.0f);
		text.setStyle(sf::Text::Bold);
		text.setPosition(sf::Vector2f(RSIZE_W - text.getLocalBounds().width - 20.0f, RSIZE_H - btnExit.getLocalBounds().height - 55.0f));
		Core()->Window()->draw(text);

		sf::RectangleShape btnRestart(sf::Vector2f(TILE_SIZE*3, TILE_SIZE));
		btnRestart.setOrigin(sf::Vector2f(0, btnExit.getLocalBounds().height/2));
		btnRestart.setPosition(sf::Vector2f(-2.0f, RSIZE_H - btnExit.getLocalBounds().height - 50.0f));
		btnRestart.setFillColor(sf::Color(65, 89, 113, 255));
		btnRestart.setOutlineThickness(2.0f);
		btnRestart.setOutlineColor(sf::Color(44, 71, 98, 255));
		Core()->Window()->draw(btnRestart);

		text.setString(CLocale::getString(RESOURCE_STR_REINICIAR));
		text.setColor(sf::Color::White);
		text.setCharacterSize(22.0f);
		text.setStyle(sf::Text::Bold);
		text.setPosition(sf::Vector2f(20.0f, RSIZE_H - btnRestart.getLocalBounds().height - 60.0f));
		Core()->Window()->draw(text);


		// Actions
		static bool tap = false;
		if (m_InputActive && sf::Touch::isDown(0) && !tap)
		{
			sf::Vector2f convCoords = Core()->Window()->mapPixelToCoords(sf::Touch::getPosition(0));
			if (btnExit.getGlobalBounds().contains(convCoords))
			{
				Core()->setPaused(false);
				Core()->SoundManager()->stopBackgroundMusic();
				Core()->SoundManager()->play(CSoundManager::SOUND_MENU_SELECT_B);
				Core()->startTransitionTo(CScreen::INIT, TRANSITION_ROLL_ZOOM_OUT);
			} else if (btnRestart.getGlobalBounds().contains(convCoords))
				startGame();

			tap = true;
		} else if (!sf::Touch::isDown(0))
			tap = false;
	} else if (m_GameState == FINISHED)
	{
		// Efecto Zoom out
		m_TextZoomNoob -= 0.25f;
		if (m_TextZoomNoob < 1.0f) m_TextZoomNoob = 1.0f;

		float ttp = USER_ZONE_LIMIT_DOWN - USER_ZONE_LIMIT_UP;
		ttp = USER_ZONE_LIMIT_UP + ttp/2;

		if (m_FinishTimer.getElapsedTime().asSeconds() > 2.3f)
			text.setString("NOOB?");
		else
			text.setString("NOOB!");
		text.setCharacterSize(162.0f);
		text.setColor(sf::Color::Red);
		text.setPosition(sf::Vector2f(RSIZE_W/2, ttp*TILE_SIZE - text.getLocalBounds().height/2));
		text.setStyle(sf::Text::Bold);
		text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
		text.setScale(m_TextZoomNoob, m_TextZoomNoob);
		Core()->Window()->draw(text);
	} else if (m_GameState == PAUSED)
	{
		// Efecto Zoom out
		m_TextZoomNoob += 0.05f;
		if (m_TextZoomNoob > 1.0f) m_TextZoomNoob = 1.0f;

		float ttp = USER_ZONE_LIMIT_DOWN - USER_ZONE_LIMIT_UP;
		ttp = USER_ZONE_LIMIT_UP + ttp/2;

		text.setString(CLocale::getString(RESOURCE_STR_PAUSADO));
		text.setCharacterSize(112.0f);
		text.setColor(sf::Color(242, 181, 15));
		text.setPosition(sf::Vector2f(RSIZE_W/2, ttp*TILE_SIZE - text.getLocalBounds().height/2));
		text.setStyle(sf::Text::Bold);
		text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
		text.setScale(m_TextZoomNoob, m_TextZoomNoob);
		Core()->Window()->draw(text);

		text.setScale(1.0f, 1.0f);
		text.setOrigin(0.0f, 0.0f);

		/*sf::RectangleShape areaButtons(sf::Vector2f(RSIZE_W, TILE_SIZE*1.5f));
		areaButtons.setPosition(sf::Vector2f(0, RSIZE_H - areaButtons.getLocalBounds().height - 70.0f));
		areaButtons.setFillColor(sf::Color(255, 255, 255, 128));
		Core()->Window()->draw(areaButtons);*/

		sf::RectangleShape btnExit(sf::Vector2f(TILE_SIZE*3, TILE_SIZE));
		btnExit.setOrigin(sf::Vector2f(0, btnExit.getLocalBounds().height/2));
		btnExit.setPosition(sf::Vector2f(-2.0f, RSIZE_H - btnExit.getLocalBounds().height - 50.0f));
		btnExit.setFillColor(sf::Color(255, 54, 54, 255));
		btnExit.setOutlineThickness(2.0f);
		btnExit.setOutlineColor(sf::Color(127, 27, 27, 255));
		Core()->Window()->draw(btnExit);

		text.setString(CLocale::getString(RESOURCE_STR_SALIR));
		text.setColor(sf::Color::White);
		text.setCharacterSize(22.0f);
		text.setStyle(sf::Text::Bold);
		text.setPosition(sf::Vector2f(20.0f, RSIZE_H - btnExit.getLocalBounds().height - 60.0f));
		Core()->Window()->draw(text);


		// Actions
		static bool tap = false;
		if (m_InputActive && sf::Touch::isDown(0) && !tap)
		{
			sf::Vector2f convCoords = Core()->Window()->mapPixelToCoords(sf::Touch::getPosition(0));
			if (btnExit.getGlobalBounds().contains(convCoords))
			{
				Core()->setPaused(false);
				Core()->SoundManager()->stopBackgroundMusic();
				Core()->SoundManager()->play(CSoundManager::SOUND_MENU_SELECT_B);
				Core()->startTransitionTo(CScreen::INIT, TRANSITION_ROLL_ZOOM_OUT);
			}

			tap = true;
		} else if (!sf::Touch::isDown(0))
			tap = false;
	}
}

void CScreenGame::setGameState(int state)
{
	m_GameState = state;
}

void CScreenGame::createPointsEffect()
{
	sf::FloatRect rect = m_HudTextScore.getGlobalBounds();

	char buff[10];
	snprintf(buff, sizeof(buff), "%09d", Core()->Player()->getPoints());
	CEffects::createTextFadeOutDown(sf::Vector2f(rect.left + rect.width/2, rect.top), buff, (Core()->Player()->getPoints()<0)?sf::Color::Red:sf::Color::Green);
}
