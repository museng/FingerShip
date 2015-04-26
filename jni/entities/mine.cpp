/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "mine.h"
#include "../engine/game_core.h"
#include "../engine/particle.h"
#include "../engine/effects.h"
#include "../engine/locale.h"
#include <cmath>

CMine::CMine(sf::Vector2f pos)
: CEntity(CEntity::MINE, pos, TILE_SIZE, TILE_SIZE, CTextureManager::TEXTURE_MINE)
{
	m_Position = pos;
	m_OrgPos = m_Position;
	m_MoveTimer = 0L;

	m_Offset = random_float(-3.0f, 3.0f);
	if (m_Offset > -1.0f && m_Offset < 1.0f) m_Offset = 1.0f;
}
CMine::~CMine()
{ }

void CMine::destroy()
{
	CEffects::createExplosion(m_Position, true);
	Core()->SoundManager()->play(CSoundManager::SOUND_DESTROY);

	CEntity::destroy();
}


void CMine::tick()
{
	m_Position.y += Core()->getGameSpeed();
	if (m_Position.y > RSIZE_H + TILE_SIZE)
	{
		setToDelete();
		return;
	}

	m_Position = sf::Vector2f(m_OrgPos.x + sinf(m_MoveTimer*0.05f)*TILE_SIZE, m_Position.y);
	m_Quad.setPosition(sf::Vector2f(m_Position.x, m_Position.y));

	if (m_Position.x < 0 || m_Position.x > RSIZE_W)
	{

		sf::Sprite spriteInd(*Core()->TextureManager()->get(CTextureManager::TEXTURE_MINE));
		spriteInd.setOrigin(spriteInd.getLocalBounds().width/2, spriteInd.getLocalBounds().height/2);
		if (m_Position.x < 0) spriteInd.setPosition(spriteInd.getLocalBounds().width/2 + 10.0f, m_Position.y);
		else spriteInd.setPosition(RSIZE_W - spriteInd.getLocalBounds().width/2 - 10.0f, m_Position.y);

		sf::CircleShape circleIndB(spriteInd.getLocalBounds().width/2, 3);
		circleIndB.setFillColor(sf::Color(255,207,23));
		circleIndB.setOrigin(spriteInd.getLocalBounds().width/2, spriteInd.getLocalBounds().height/2);
		if (m_Position.x < 0)
		{
			circleIndB.setPosition(spriteInd.getLocalBounds().width/2, m_Position.y);
			circleIndB.setRotation(270);
		}
		else
		{
			circleIndB.setPosition(RSIZE_W - spriteInd.getLocalBounds().width/2, m_Position.y);
			circleIndB.setRotation(90);
		}
		Core()->Window()->draw(circleIndB);

		sf::CircleShape circleInd(spriteInd.getLocalBounds().width/2);
		circleInd.setFillColor(sf::Color(255,207,23));
		circleInd.setOrigin(spriteInd.getLocalBounds().width/2, spriteInd.getLocalBounds().height/2);
		if (m_Position.x < 0) circleInd.setPosition(spriteInd.getLocalBounds().width/2 + 10.0f, m_Position.y);
		else circleInd.setPosition(RSIZE_W - spriteInd.getLocalBounds().width/2 - 10.0f, m_Position.y);

		Core()->Window()->draw(circleInd);
		Core()->Window()->draw(spriteInd);
	}

	Core()->Window()->draw(m_Quad);

    if (!Core()->m_MineKilled)
    {
    	sf::RectangleShape rectShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    	rectShape.setPosition(sf::Vector2f(m_Position.x, m_Position.y));
    	rectShape.setFillColor(sf::Color::Transparent);
    	rectShape.setOutlineThickness(2.0f);
    	rectShape.setOutlineColor(sf::Color::Red);
    	rectShape.setOrigin(sf::Vector2f(TILE_SIZE/2, TILE_SIZE/2));
    	Core()->Window()->draw(rectShape);

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(m_Position.x - TILE_SIZE/2, m_Position.y - TILE_SIZE/2 - 1.0f), sf::Color::Red),
            sf::Vertex(sf::Vector2f(m_Position.x - TILE_SIZE/2 + 200.0f, m_Position.y - TILE_SIZE/2 - 1.0f), sf::Color::Red)
        };
        Core()->Window()->draw(line, 2, sf::Lines);

        sf::Text text;
        text.setString(CLocale::getString(RESOURCE_STR_DISPARALE_PARA_DESTRUILA));
        text.setFont(Core()->getDefaultFont());
        text.setCharacterSize(20.0f);
        text.setOrigin(0, text.getLocalBounds().height);
        text.setPosition(sf::Vector2f(m_Position.x - TILE_SIZE/2, m_Position.y - TILE_SIZE/2 - 5.0f));
        text.setColor(sf::Color::Cyan);
        Core()->Window()->draw(text);
    }

    if (!Core()->isPaused())
    	m_MoveTimer++;
}
