/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "screen_credits.h"
#include "../engine/game_core.h"
#include "../engine/locale.h"
#include "../engine/effects.h"
#include <cmath>

#define MAX_STR_CREDITS 	8


CScreenCredits::CScreenCredits(int camW, int camH)
: CScreen(camW, camH)
{
	Core()->setBackgroundColor(sf::Color::Black);
	m_StrIndex = 0;
	m_End = false;
	m_TextZoom = 0.0f;
}
CScreenCredits::~CScreenCredits()
{ }

void CScreenCredits::tick()
{
	// TODO: ugly
	char m_sCredits[MAX_STR_CREDITS*2][255] = {
		"\0",	"joshuaempyre\0",
		"\0",	"SpellingPhailer\0",
		"\0",	"www.freesound.org\0",
		"\0",	"Kenney Vleugels\0",
		"\0",	"http://hasgraphics.com\0",
		"\0",	"TakuTag [Vandal]\0",
		"\0",	"Juan McKernel\0",
		"\0",	"Alexandre Diaz\0"
	};
	strncpy(m_sCredits[0], CLocale::getString(RESOURCE_STR_MUSICA), sizeof(m_sCredits[0]));
	strncpy(m_sCredits[2], CLocale::getString(RESOURCE_STR_MUSICA), sizeof(m_sCredits[2]));
	strncpy(m_sCredits[4], CLocale::getString(RESOURCE_STR_SONIDOS), sizeof(m_sCredits[4]));
	strncpy(m_sCredits[6], CLocale::getString(RESOURCE_STR_GRAFICOS), sizeof(m_sCredits[6]));
	strncpy(m_sCredits[8], CLocale::getString(RESOURCE_STR_GRAFICOS), sizeof(m_sCredits[8]));
	strncpy(m_sCredits[10], CLocale::getString(RESOURCE_STR_LOGO), sizeof(m_sCredits[10]));
	strncpy(m_sCredits[12], CLocale::getString(RESOURCE_STR_BETA_TESTER), sizeof(m_sCredits[12]));
	strncpy(m_sCredits[14], CLocale::getString(RESOURCE_STR_AUTOR_PROGRAMACION), sizeof(m_sCredits[14]));
	//

	if (!m_End && m_Timer.getElapsedTime().asSeconds() > 4.32f)
	{
		if (m_StrIndex >= MAX_STR_CREDITS*2-2)
		{
			m_End = true;

			if (Core()->getLastScreenId() == CScreen::GAME)
			{
				Core()->setScreen(CScreen::GAME_FINISH);
				return;
			}
			else
				Core()->startScreenTransitionTo(CScreen::INIT, TRANSITION_SLIDE_L);
		}
		else
		{
			m_TextZoom = 0.0f;
			m_StrIndex+=2;
			m_Timer.restart();
		}
	}

	sf::Text text;
	text.setFont(Core()->getDefaultFont());

	CScreen::renderBack();

	text.setString(CLocale::getString(RESOURCE_STR_CREDITOS));
	text.setColor(sf::Color::White);
	text.setCharacterSize(75.0f);
	text.setPosition(RSIZE_W/2 - text.getLocalBounds().width/2, 70.0f);
	Core()->Window()->draw(text);

	text.setString(m_sCredits[m_StrIndex]);
	text.setColor(sf::Color(134, 43, 84));
	text.setCharacterSize(45.0f);
	text.setPosition(RSIZE_W/2 - text.getLocalBounds().width/2, RSIZE_H/2-text.getLocalBounds().height/2 - 20.0f);
	Core()->Window()->draw(text);

	// Efecto Zoom out
	m_TextZoom += 0.05f;
	if (m_TextZoom > 1.0f) m_TextZoom = 1.0f;
	text.setScale(m_TextZoom, m_TextZoom);

	float dt = m_Timer.getElapsedTime().asSeconds() * 1.35f;
	text.setString(m_sCredits[m_StrIndex + 1]);
	text.setColor(sf::Color(176, 133, 99));
	text.setCharacterSize(50.0f);
	text.setRotation(sinf(dt)*15.0f);
	text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
	text.setPosition(RSIZE_W/2, RSIZE_H/2 + 50.0f);
	Core()->Window()->draw(text);


	CScreen::render();
	CScreen::renderFront();
}

sf::View& CScreenCredits::updateCamera()
{
	return *Camera();
}
