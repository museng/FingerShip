/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "enemy_ship.h"
#include "../engine/game_core.h"
#include "../engine/locale.h"
#include "../engine/effects.h"
#include <cmath>

CEnemyShipA::CEnemyShipA(sf::Vector2f pos, float offset)
: CEntity(CEntity::ENEMY_SHIP_A)
{
	m_Position = pos;
	m_OrgPos = m_Position;
	m_MoveTimer = 0L;

	sf::Vector2f size(TILE_SIZE, TILE_SIZE);
	m_CollChar = sf::RectangleShape(sf::Vector2f(size.x, size.y));
	m_CollChar.setOrigin(sf::Vector2f(size.x/2, size.y/2));
	m_CollChar.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_SHIP_ENEMY));

	m_Offset = offset;
}
CEnemyShipA::~CEnemyShipA()
{ }

void CEnemyShipA::destroy()
{
	CEffects::createExplosion(m_Position, false);
	Core()->SoundManager()->play(CSoundManager::SOUND_DESTROY);

	CEntity::destroy();
}


void CEnemyShipA::tick()
{
	m_Position.y += Core()->getGameSpeed() + 3.0f;
	if (m_Position.y > RSIZE_H + TILE_SIZE)
	{
		setToDelete();
		return;
	}

	m_Position = sf::Vector2f(m_OrgPos.x + sinf(m_MoveTimer*0.03f*m_Offset)*128, m_Position.y);
	m_CollChar.setPosition(sf::Vector2f(m_Position.x, m_Position.y));


	if (m_Position.x < 0 || m_Position.x > RSIZE_W)
	{

		sf::Sprite spriteInd(*Core()->TextureManager()->get(CTextureManager::TEXTURE_SHIP_ENEMY));
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
		spriteInd.setScale(0.75f, 0.75f);
		Core()->Window()->draw(spriteInd);
	}

	Core()->Window()->draw(m_CollChar);

    if (!Core()->m_EnemyShipKilled)
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
