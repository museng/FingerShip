/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "spaceman.h"
#include "../engine/game_core.h"
#include "../engine/locale.h"
#include "../engine/basic_functions.h"
#include "../engine/effects.h"
#include <cmath>


static const sf::IntRect m_SpacemanMovements[3] = {
	{ 0, 0, 14, 14 },
	{ 14, 0, 28, 14 },
	{ 28, 0, 42, 14 }
};

CSpaceman::CSpaceman(sf::Vector2f pos)
: CEntity(CEntity::SPACEMAN, pos, TILE_SIZE/2, TILE_SIZE/2, CTextureManager::TEXTURE_SPACEMAN)
{
	m_Position = pos;

	sf::Vector2f rawDir = Core()->Player()->m_pCharacter->getPosition() - m_Position;
	m_Dir = sf::Vector2f(rawDir.x, rawDir.y);
	m_Dir = vector_normalize(m_Dir);

	m_Vel = random_int(0, 1);
	m_RotDir = random_int(-1, 1);

	m_AnimTimer = 0L;
	m_AnimIndex = 2;

	m_Quad.setTextureRect(m_SpacemanMovements[m_AnimIndex]);
}
CSpaceman::~CSpaceman()
{ }

void CSpaceman::destroy()
{
	Core()->SoundManager()->play(CSoundManager::SOUND_SPACEMAN_DEAD);
	CEffects::createBloodSplash(m_Position);

	CEntity::destroy();
}


void CSpaceman::tick()
{
	float vel = Core()->getGameSpeed();
	m_Position = sf::Vector2f(m_Position.x + m_Dir.x * vel, m_Position.y + m_Dir.y * vel);

	if (m_Position.y > RSIZE_H + m_Quad.getLocalBounds().height || m_Position.x+m_Quad.getLocalBounds().width/2 < 0 || m_Position.x-m_Quad.getLocalBounds().width/2 > RSIZE_W)
	{
		setToDelete();
		return;
	}

	if (!Core()->isPaused())
	{
		m_Quad.setPosition(sf::Vector2f(m_Position.x, m_Position.y));
		m_Quad.setRotation(m_Quad.getRotation()+m_RotDir);
	}

	if (m_AnimTimer > 10L)
	{
		m_AnimIndex++;
		if (m_AnimIndex >= 3) m_AnimIndex = 0;

		m_Quad.setTextureRect(m_SpacemanMovements[m_AnimIndex]);
		m_AnimTimer = 0L;
	}
	Core()->Window()->draw(m_Quad);

    if (!Core()->m_SpacemanRescue)
    {
    	sf::RectangleShape rectShape(sf::Vector2f(m_Quad.getLocalBounds().width, m_Quad.getLocalBounds().height));
    	rectShape.setPosition(sf::Vector2f(m_Position.x, m_Position.y));
    	rectShape.setFillColor(sf::Color::Transparent);
    	rectShape.setOutlineThickness(2.0f);
    	rectShape.setOutlineColor(sf::Color::Green);
    	rectShape.setOrigin(sf::Vector2f(m_Quad.getLocalBounds().width/2, m_Quad.getLocalBounds().height/2));
    	Core()->Window()->draw(rectShape);

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(m_Position.x - m_Quad.getLocalBounds().width/2, m_Position.y - m_Quad.getLocalBounds().height/2 - 1.0f), sf::Color::Green),
            sf::Vertex(sf::Vector2f(m_Position.x - m_Quad.getLocalBounds().width/2 + 200.0f, m_Position.y - m_Quad.getLocalBounds().height/2 - 1.0f), sf::Color::Green)
        };
        Core()->Window()->draw(line, 2, sf::Lines);

        sf::Text text;
        text.setString(CLocale::getString(RESOURCE_STR_RECOGELO_PARA_AYUDARLE));
        text.setFont(Core()->getDefaultFont());
        text.setCharacterSize(20.0f);
        text.setOrigin(0, text.getLocalBounds().height);
        text.setPosition(sf::Vector2f(m_Position.x - m_Quad.getLocalBounds().width/2, m_Position.y - m_Quad.getLocalBounds().height/2 - 5.0f));
        text.setColor(sf::Color::Cyan);
        Core()->Window()->draw(text);
    }

    if (!Core()->isPaused())
    	m_AnimTimer++;
}
