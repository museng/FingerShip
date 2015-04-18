/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "screen_options.h"
#include "../engine/game_core.h"
#include "../engine/locale.h"
#include "../engine/effects.h"

CScreenOptions::CScreenOptions(int camW, int camH)
: CScreen(camW, camH)
{
	Core()->setBackgroundColor(sf::Color::Black);

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
CScreenOptions::~CScreenOptions()
{ }

void CScreenOptions::tick()
{
	sf::Text text, textLowGraph, textSounds, textMusic;
	text.setFont(Core()->getDefaultFont());
	textLowGraph.setFont(Core()->getDefaultFont());
	textSounds.setFont(Core()->getDefaultFont());
	textMusic.setFont(Core()->getDefaultFont());

	if (!Core()->m_Config.m_LowGraphics)
	{
		for (int i=0; i<RSIZE_W; i++)
		{
			if (random_int(0, 1000) == 5)
			{
				CParticleStar *pStar = new CParticleStar();
				pStar->m_Pos = sf::Vector2f(i, -TILE_SIZE);
				addParticle(pStar);
			}
		}
	}

	CScreen::renderBack();
	CScreen::renderFront();

	sf::RectangleShape btnBack(sf::Vector2f(TILE_SIZE*3, TILE_SIZE));
	btnBack.setOrigin(sf::Vector2f(0, btnBack.getLocalBounds().height/2));
	btnBack.setPosition(sf::Vector2f(-2.0f, RSIZE_H - btnBack.getLocalBounds().height - 80.0f));
	btnBack.setFillColor(sf::Color(65, 89, 113, 255));
	btnBack.setOutlineThickness(2.0f);
	btnBack.setOutlineColor(sf::Color(44, 71, 98, 255));
	Core()->Window()->draw(btnBack);

	text.setString(CLocale::getString(RESOURCE_STR_ATRAS));
	text.setColor(sf::Color::White);
	text.setCharacterSize(22.0f);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(10.0f, RSIZE_H - btnBack.getLocalBounds().height - 85.0f));
	Core()->Window()->draw(text);


	sf::RectangleShape checkLowGraphs(sf::Vector2f(50.0f, 50.0f));
	checkLowGraphs.setPosition(50.0f, 150.0f);
	checkLowGraphs.setFillColor(sf::Color::Black);
	checkLowGraphs.setOutlineThickness(2.0f);
	checkLowGraphs.setOutlineColor(sf::Color::Cyan);
	Core()->Window()->draw(checkLowGraphs);

	if (Core()->m_Config.m_LowGraphics)
	{
		sf::RectangleShape checkLowGraphsSel(sf::Vector2f(40.0f, 40.0f));
		checkLowGraphsSel.setPosition(55.0f, 155.0f);
		checkLowGraphsSel.setFillColor(sf::Color::Cyan);
		Core()->Window()->draw(checkLowGraphsSel);
	}

	textLowGraph.setString(CLocale::getString(RESOURCE_STR_REDUCIR_GRAFICOS));
	textLowGraph.setColor(sf::Color::Cyan);
	textLowGraph.setCharacterSize(42.0f);
	textLowGraph.setStyle(sf::Text::Bold);
	textLowGraph.setPosition(sf::Vector2f(120.0f, checkLowGraphs.getGlobalBounds().top + 10.0f));
	Core()->Window()->draw(textLowGraph);

	sf::RectangleShape checkSounds(sf::Vector2f(50.0f, 50.0f));
	checkSounds.setPosition(50.0f, 250.0f);
	checkSounds.setFillColor(sf::Color::Black);
	checkSounds.setOutlineThickness(2.0f);
	checkSounds.setOutlineColor(sf::Color::Cyan);
	Core()->Window()->draw(checkSounds);

	if (Core()->SoundManager()->isSfxActive())
	{
		sf::RectangleShape checkSoundsSel(sf::Vector2f(40.0f, 40.0f));
		checkSoundsSel.setPosition(55.0f, 255.0f);
		checkSoundsSel.setFillColor(sf::Color::Cyan);
		Core()->Window()->draw(checkSoundsSel);
	}

	textSounds.setString(CLocale::getString(RESOURCE_STR_SONIDOS));
	textSounds.setColor(sf::Color::Cyan);
	textSounds.setCharacterSize(42.0f);
	textSounds.setStyle(sf::Text::Bold);
	textSounds.setPosition(sf::Vector2f(120.0f, checkSounds.getGlobalBounds().top + 10.0f));
	Core()->Window()->draw(textSounds);

	sf::RectangleShape checkMusic(sf::Vector2f(50.0f, 50.0f));
	checkMusic.setPosition(50.0f, 330.0f);
	checkMusic.setFillColor(sf::Color::Black);
	checkMusic.setOutlineThickness(2.0f);
	checkMusic.setOutlineColor(sf::Color::Cyan);
	Core()->Window()->draw(checkMusic);

	if (Core()->SoundManager()->isMusicActive())
	{
		sf::RectangleShape checkSoundsSel(sf::Vector2f(40.0f, 40.0f));
		checkSoundsSel.setPosition(55.0f, 335.0f);
		checkSoundsSel.setFillColor(sf::Color::Cyan);
		Core()->Window()->draw(checkSoundsSel);
	}

	textMusic.setString(CLocale::getString(RESOURCE_STR_MUSICA));
	textMusic.setColor(sf::Color::Cyan);
	textMusic.setCharacterSize(42.0f);
	textMusic.setStyle(sf::Text::Bold);
	textMusic.setPosition(sf::Vector2f(120.0f, checkMusic.getGlobalBounds().top + 10.0f));
	Core()->Window()->draw(textMusic);

	text.setString(CLocale::getString(RESOURCE_STR_IDIOMA));
	text.setColor(sf::Color::Cyan);
	text.setCharacterSize(42.0f);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(50.0f, 440.0f));
	Core()->Window()->draw(text);

	text.setCharacterSize(42.0f);
	text.setStyle(sf::Text::Bold);

	sf::RectangleShape langES(sf::Vector2f(RSIZE_W, 92.0f));
	langES.setPosition(0.0f, 490.0f);
	if (Core()->m_Config.m_Language == LANGUAGE_ES)
	{
		langES.setFillColor(sf::Color::Cyan);
		text.setColor(sf::Color::Black);
	}
	else
	{
		langES.setFillColor(sf::Color::Transparent);
		text.setColor(sf::Color::Cyan);
	}
	Core()->Window()->draw(langES);
	text.setString("CASTELLANO [ES]");
	text.setPosition(sf::Vector2f(RSIZE_W - text.getLocalBounds().width - 10.0f, langES.getGlobalBounds().top + langES.getLocalBounds().height/2 - text.getLocalBounds().height/2 - 5.0f));
	Core()->Window()->draw(text);

	sf::RectangleShape langUS(sf::Vector2f(RSIZE_W, 92.0f));
	langUS.setPosition(0.0f, 580.0f);
	langUS.setFillColor(sf::Color::Transparent);
	if (Core()->m_Config.m_Language == LANGUAGE_US)
	{
		langUS.setFillColor(sf::Color::Cyan);
		text.setColor(sf::Color::Black);
	}
	else
	{
		langUS.setFillColor(sf::Color::Transparent);
		text.setColor(sf::Color::Cyan);
	}
	Core()->Window()->draw(langUS);
	text.setString("ENGLISH [EN]");
	text.setPosition(sf::Vector2f(RSIZE_W - text.getLocalBounds().width - 10.0f, langUS.getGlobalBounds().top + langUS.getLocalBounds().height/2 - text.getLocalBounds().height/2 - 5.0f));
	Core()->Window()->draw(text);

	sf::RectangleShape langGAL(sf::Vector2f(RSIZE_W, 92.0f));
	langGAL.setPosition(0.0f, 670.0f);
	langGAL.setFillColor(sf::Color::Transparent);
	if (Core()->m_Config.m_Language == LANGUAGE_GAL)
	{
		langGAL.setFillColor(sf::Color::Cyan);
		text.setColor(sf::Color::Black);
	}
	else
	{
		langUS.setFillColor(sf::Color::Transparent);
		text.setColor(sf::Color::Cyan);
	}
	Core()->Window()->draw(langGAL);
	text.setString("GALEGO [GAL]");
	text.setPosition(sf::Vector2f(RSIZE_W - text.getLocalBounds().width - 10.0f, langGAL.getGlobalBounds().top + langGAL.getLocalBounds().height/2 - text.getLocalBounds().height/2 - 5.0f));
	Core()->Window()->draw(text);

	// Powered By
	text.setString("Powered By");
	text.setColor(sf::Color(255, 255, 255, 128));
	text.setCharacterSize(32.0f);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(RSIZE_W-262, RSIZE_H - 95 - text.getLocalBounds().height));
	Core()->Window()->draw(text);

	sf::RectangleShape sfml(sf::Vector2f(252, 81));
	sfml.setPosition(sf::Vector2f(RSIZE_W-262, RSIZE_H - 91));
	sfml.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_SFML));
	sfml.setFillColor(sf::Color(255, 255, 255, 128));
	Core()->Window()->draw(sfml);


	// Actions in Screen
	static bool tap = false;
    if (m_InputActive && sf::Touch::isDown(0) && !tap)
    {
    	sf::Vector2f convCoords = Core()->Window()->mapPixelToCoords(sf::Touch::getPosition(0));
    	if (btnBack.getGlobalBounds().contains(convCoords))
    	{
    		Core()->SoundManager()->play(CSoundManager::SOUND_MENU_SELECT_B);
    		Core()->startTransitionTo(CScreen::INIT, TRANSITION_SLIDE_R);
    	}
    	else if (checkLowGraphs.getGlobalBounds().contains(convCoords) || textLowGraph.getGlobalBounds().contains(convCoords))
    	{
    		Core()->SoundManager()->play(CSoundManager::SOUND_MENU_CHECK);
    		Core()->m_Config.m_LowGraphics = !Core()->m_Config.m_LowGraphics;
    	}
    	else if (checkSounds.getGlobalBounds().contains(convCoords) || textSounds.getGlobalBounds().contains(convCoords))
    	{
    		Core()->SoundManager()->play(CSoundManager::SOUND_MENU_CHECK);
    		Core()->SoundManager()->setSfxActive(!Core()->SoundManager()->isSfxActive());
    	}
    	else if (checkMusic.getGlobalBounds().contains(convCoords) || textMusic.getGlobalBounds().contains(convCoords))
    	{
    		Core()->SoundManager()->play(CSoundManager::SOUND_MENU_CHECK);
    		Core()->SoundManager()->setMusicActive(!Core()->SoundManager()->isMusicActive());
    	}
    	else if (langES.getGlobalBounds().contains(convCoords))
    	{
    		Core()->SoundManager()->play(CSoundManager::SOUND_MENU_CHECK);
    		Core()->m_Config.m_Language = LANGUAGE_ES;
    	}
    	else if (langUS.getGlobalBounds().contains(convCoords))
    	{
    		Core()->SoundManager()->play(CSoundManager::SOUND_MENU_CHECK);
    		Core()->m_Config.m_Language = LANGUAGE_US;
    	}
    	else if (langGAL.getGlobalBounds().contains(convCoords))
    	{
    		Core()->SoundManager()->play(CSoundManager::SOUND_MENU_CHECK);
    		Core()->m_Config.m_Language = LANGUAGE_GAL;
    	}

    	tap = true;
    } else if (!sf::Touch::isDown(0))
    	tap = false;
    //
}

sf::View& CScreenOptions::updateCamera()
{
	//Camera()->rotate(1.0f);
	return *Camera();
}
