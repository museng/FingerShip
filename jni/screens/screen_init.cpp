/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "screen_init.h"
#include "../engine/game_core.h"
#include "../engine/locale.h"
#include "../engine/effects.h"
#include "../engine/android_utils.h"
#include <cmath>

CScreenInit::CScreenInit(int camW, int camH)
: CScreen(camW, camH)
{
	Core()->SoundManager()->playBackgroundMusic(CSoundManager::MUSIC_INTRO);
	Core()->setBackgroundColor(sf::Color::Black);

	// Started Stars...
	if (!Core()->m_Config.m_LowGraphics)
		addInitStars();
}
CScreenInit::~CScreenInit()
{ }

void CScreenInit::tick()
{
	static bool anim = true;
	static sf::Color colorHand = sf::Color::White;
	static sf::Clock timerTextAnim;
	static float textOffset = -400.0f;
	if (anim)
	{
		textOffset += 10.0f;

		if (textOffset > 0.0f)
		{
			textOffset = 0.0f;
			anim = false;
			Core()->SoundManager()->play(CSoundManager::SOUND_INTRO);
		}
	}
	if (!anim && colorHand.a > 0)
		colorHand.a -= 10.0f;

	sf::Text text, textCredits;
	text.setFont(Core()->getDefaultFont());
	textCredits.setFont(Core()->getDefaultFont());

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
	CScreen::render();
	CScreen::renderFront();

	text.setString("FingerShip");
	text.setColor(sf::Color::Red);
	text.setCharacterSize(72);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(RSIZE_W/2 - text.getLocalBounds().width/2 + textOffset, 100.0f));
	Core()->Window()->draw(text);

	if (colorHand.a > 0.0f)
	{
		sf::RectangleShape imgHand(sf::Vector2f(128, 99));
		imgHand.setOrigin(sf::Vector2f(imgHand.getLocalBounds().width, imgHand.getLocalBounds().height/2));
		imgHand.setPosition(sf::Vector2f(RSIZE_W/2 - text.getLocalBounds().width/2 + textOffset - 10.0f, 150.0f));
		imgHand.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_HAND));
		imgHand.setFillColor(colorHand);
		Core()->Window()->draw(imgHand);
	}


	if (!anim)
	{
		int off = text.getGlobalBounds().left + text.getLocalBounds().width;
		text.setString(GAME_VERSION);
		text.setColor(sf::Color(241, 229, 122, 255-colorHand.a));
		text.setCharacterSize(32.0f);
		text.setStyle(sf::Text::Bold);
		text.setPosition(sf::Vector2f(off-text.getLocalBounds().width-5, 100.0f - text.getLocalBounds().height));
		Core()->Window()->draw(text);

		sf::Color textRetoColor = sf::Color::Cyan;
		textRetoColor.a = 255-colorHand.a;
		char buff[128];
		snprintf(buff, sizeof(buff), CLocale::getString(RESOURCE_STR_EL_RETO_DE_LOS_X_METROS), MAX_METERS);
		text.setString(buff);
		text.setColor(textRetoColor);
		text.setCharacterSize(16.0f);
		text.setStyle(sf::Text::Bold);
		text.setPosition(sf::Vector2f(RSIZE_W/2 - text.getLocalBounds().width/2, 180.0f));
		Core()->Window()->draw(text);
	}

	text.setString(CLocale::getString(RESOURCE_STR_PULSA_EN_LA_NAVE_PARA_EMPEZAR));
	text.setCharacterSize(22.0f);
	text.setColor(sf::Color::Green);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(RSIZE_W/2 - text.getLocalBounds().width/2, RSIZE_H/2 - text.getLocalBounds().height/2));
	Core()->Window()->draw(text);

	textCredits.setString(CLocale::getString(RESOURCE_STR_CREADO_POR_UNSIGNED_CHAR));
	textCredits.setColor(sf::Color::White);
	textCredits.setCharacterSize(16.0f);
	textCredits.setStyle(sf::Text::Bold);
	textCredits.setPosition(sf::Vector2f(RSIZE_W/2 - textCredits.getLocalBounds().width/2, RSIZE_H - textCredits.getLocalBounds().height - 90.0f));
	Core()->Window()->draw(textCredits);

	sf::RectangleShape btnOptions(sf::Vector2f(TILE_SIZE*3, TILE_SIZE));
	btnOptions.setOrigin(sf::Vector2f(btnOptions.getLocalBounds().width/2, btnOptions.getLocalBounds().height/2));
	btnOptions.setPosition(sf::Vector2f(RSIZE_W/2, 280.0f));
	btnOptions.setFillColor(sf::Color(65, 89, 113, 255));
	btnOptions.setOutlineThickness(2.0f);
	btnOptions.setOutlineColor(sf::Color(44, 71, 98, 255));
	Core()->Window()->draw(btnOptions);

	text.setString(CLocale::getString(RESOURCE_STR_OPCIONES));
	text.setColor(sf::Color::White);
	text.setCharacterSize(22.0f);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(RSIZE_W/2 - text.getLocalBounds().width/2, 270.0f));
	Core()->Window()->draw(text);

	sf::RectangleShape btnClose(sf::Vector2f(TILE_SIZE*3, TILE_SIZE));
	btnClose.setOrigin(sf::Vector2f(btnOptions.getLocalBounds().width/2, btnOptions.getLocalBounds().height/2));
	btnClose.setPosition(sf::Vector2f(RSIZE_W/2, 380.0f));
	btnClose.setFillColor(sf::Color(255, 54, 54, 255));
	btnClose.setOutlineThickness(2.0f);
	btnClose.setOutlineColor(sf::Color(127, 27, 27, 255));
	Core()->Window()->draw(btnClose);

	text.setString(CLocale::getString(RESOURCE_STR_CERRAR));
	text.setColor(sf::Color::White);
	text.setCharacterSize(22.0f);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(RSIZE_W/2 - text.getLocalBounds().width/2, 370.0f));
	Core()->Window()->draw(text);

	// Render Character Init
	sf::Vector2f size(CCharacter::SIZE, CCharacter::SIZE);
	sf::CircleShape character = sf::CircleShape(size.x); // Radio
	character.setOrigin(sf::Vector2f(size.x, size.y));
	character.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_SHIP));

	int offset = TILE_SIZE*USER_ZONE_LIMIT_DOWN - TILE_SIZE*USER_ZONE_LIMIT_UP;
	offset = TILE_SIZE*USER_ZONE_LIMIT_UP + offset/2;

	float dt = Core()->getGameTime().asSeconds();
	sf::Vector2f posChar = sf::Vector2f(RSIZE_W/2 + sinf(dt)*12.0f, offset + + cosf(dt)*12.0f);
	character.setPosition(posChar);
	CEffects::createFireTrail(sf::Vector2f(posChar.x, posChar.y));
	Core()->Window()->draw(character);

	// Actions in Screen
	static short credits_counts = 0;
	static bool tap = false;
    if (m_InputActive && sf::Touch::isDown(0) && !tap)
    {
    	sf::Vector2f convCoords = Core()->Window()->mapPixelToCoords(sf::Touch::getPosition(0));
    	if (character.getGlobalBounds().contains(convCoords))
    		Core()->setScreen(CScreen::GAME);
    	else if (btnOptions.getGlobalBounds().contains(convCoords))
    	{
    		Core()->startScreenTransitionTo(CScreen::OPTIONS, TRANSITION_SLIDE_L);
    		Core()->SoundManager()->play(CSoundManager::SOUND_MENU_SELECT_B);
    	}
    	else if (btnClose.getGlobalBounds().contains(convCoords))
    		Core()->Window()->close();

    	if (textCredits.getGlobalBounds().contains(convCoords))
    	{
    		credits_counts++;
    		if (credits_counts == 3)
				Core()->startScreenTransitionTo(CScreen::CREDITS, TRANSITION_SLIDE_R);

    		Core()->SoundManager()->play(CSoundManager::SOUND_MENU_SELECT_B);
    	} else
    		credits_counts = 0;

    	tap = true;
    } else if (!sf::Touch::isDown(0))
    	tap = false;
    //

}

sf::View& CScreenInit::updateCamera()
{
	return *Camera();
}
