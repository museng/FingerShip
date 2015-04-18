/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "screen_game_finished.h"
#include "../engine/game_core.h"
#include "../engine/locale.h"
#include "../engine/effects.h"
#include <cmath>

CScreenGameFinished::CScreenGameFinished(int camW, int camH)
: CScreen(camW, camH)
{
	Core()->setBackgroundColor(sf::Color::Black);
}
CScreenGameFinished::~CScreenGameFinished()
{ }

void CScreenGameFinished::tick()
{
	sf::Text text;
	text.setFont(Core()->getDefaultFont());

	if (m_FireworksTimer.getElapsedTime().asMilliseconds() > 675)
	{
		Core()->SoundManager()->play(CSoundManager::SOUND_FIREWORK);
		CEffects::createFirework(sf::Vector2f(random_int(0, RSIZE_W), random_int(0, RSIZE_H)));
		m_FireworksTimer.restart();
	}

	CScreen::renderBack();
	CScreen::renderFront();

	char buff[15];
	snprintf(buff, sizeof(buff), "%09d", Core()->Player()->getPoints());
	text.setString(buff);
	text.setColor(sf::Color::Cyan);
	text.setCharacterSize(72);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(RSIZE_W/2 - text.getLocalBounds().width/2, 180.0f));
	Core()->Window()->draw(text);

	float dt = m_Timer.getElapsedTime().asSeconds() * 1.35f;
	text.setString(CLocale::getString(RESOURCE_STR_HAS_GANADO));
	text.setColor(sf::Color::Green);
	text.setCharacterSize(72);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(RSIZE_W/2, RSIZE_H/2 - text.getLocalBounds().height/2));
	text.setRotation(sinf(dt)*15.0f);
	text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
	Core()->Window()->draw(text);

	text.setString(CLocale::getString(RESOURCE_STR_SI_TE_HA_GUSTADO_POR_FAVOR));
	text.setColor(sf::Color::Yellow);
	text.setCharacterSize(32);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(RSIZE_W/2, (RSIZE_H/2 - text.getLocalBounds().height/2)+75.0f));
	text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
	Core()->Window()->draw(text);

	text.setString(CLocale::getString(RESOURCE_STR_VOTA_EN_LA_STORE_GRACIAS));
	text.setCharacterSize(32);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(RSIZE_W/2, (RSIZE_H/2 - text.getLocalBounds().height/2)+120.0f));
	text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
	Core()->Window()->draw(text);

	text.setRotation(0.0f);
	text.setOrigin(0.0f, 0.0f);

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

	// Actions in Screen
	static bool tap = false;
    if (m_InputActive && sf::Touch::isDown(0) && !tap)
    {
    	sf::Vector2f convCoords = Core()->Window()->mapPixelToCoords(sf::Touch::getPosition(0));
    	if (btnExit.getGlobalBounds().contains(convCoords))
    	{
    		Core()->SoundManager()->play(CSoundManager::SOUND_MENU_SELECT_B);
    		Core()->setScreen(CScreen::INIT);
    	} else
    	{
    		Core()->SoundManager()->play(CSoundManager::SOUND_FIREWORK);
    		CEffects::createFirework(convCoords);
    	}

    	tap = true;
    } else if (!sf::Touch::isDown(0))
    	tap = false;
}

sf::View& CScreenGameFinished::updateCamera()
{
	return *Camera();
}
