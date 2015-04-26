/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "turret.h"
#include "projectile.h"
#include "../engine/game_core.h"
#include "../engine/effects.h"
#include "../engine/locale.h"
#include <cmath>

CTurret::CTurret(sf::Vector2f pos)
: CEntity(CEntity::TURRET, pos, TILE_SIZE, TILE_SIZE, CTextureManager::TEXTURE_TURRET)
{
	m_Alive = true;
	m_OrgPos = m_Position = pos;
	m_Shoot = false;
	m_ShootTime = 0L;
}
CTurret::~CTurret()
{ }

void CTurret::destroy()
{
	CEffects::createExplosion(m_Position, false);
	Core()->SoundManager()->play(CSoundManager::SOUND_DESTROY);

	CEntity::destroy();
}


void CTurret::tick()
{
	m_Position.y += Core()->getGameSpeed();
	m_OrgPos.y += Core()->getGameSpeed();
	if (m_Position.y > RSIZE_H + TILE_SIZE)
	{
		setToDelete();
		return;
	}

	if (m_Alive && !Core()->isPaused())
	{
		if (sf::Touch::isDown(0))
		{
			sf::Vector2f convCoords = Core()->Window()->mapPixelToCoords(sf::Touch::getPosition(0));
			if (Core()->Player()->m_pCharacter && m_Quad.getGlobalBounds().contains(convCoords))
			{
				CEffects::createPoints(m_Position, 250);
				Core()->Player()->addPoints(250);
				CEffects::createExplosion(m_Position, false);
				Core()->SoundManager()->play(CSoundManager::SOUND_EXPLOSION);
				Core()->SoundManager()->play(CSoundManager::SOUND_DESTROY);
				m_Alive = false;
				m_Quad.setFillColor(sf::Color(45, 45, 45, 255));

				Core()->m_TurretKilled = true;
			}
		}

		if (Core()->Player()->m_pCharacter)
		{
			sf::Vector2f rawDir = Core()->Player()->m_pCharacter->getPosition() - m_Position;
			sf::Vector2f dir = sf::Vector2f(rawDir.x, rawDir.y);
			dir = vector_normalize(dir);

			m_Quad.setRotation(-vector_angle(dir));

			if (m_ShootTime > 65L)
			{
				Core()->SoundManager()->play(CSoundManager::SOUND_TURRET_SHOOT);
				Core()->getScreen()->addEntity(new CProjectile(getType(), sf::Vector2f(m_Position.x, m_Position.y) + sf::Vector2f(dir.x * 21, dir.y * 21), sf::Color::Blue, dir, 10.0f));

				m_Shoot = true;
				m_OrgPos = m_Position;
				m_Position = m_Position - sf::Vector2f(dir.x * 5, dir.y * 5);
				m_ShootTime = 0L;
			}
			else if (m_Shoot && m_ShootTime > 3L)
			{
				m_Position = m_OrgPos;
				m_Shoot = false;
			}

			m_ShootTime++;
		}
	}

	m_Quad.setPosition(sf::Vector2f(m_Position.x, m_Position.y));
	Core()->Window()->draw(m_Quad);

    if (!Core()->m_TurretKilled)
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
            sf::Vertex(sf::Vector2f(m_Position.x - TILE_SIZE/2, m_Position.y - 1.0f - TILE_SIZE/2), sf::Color::Red),
            sf::Vertex(sf::Vector2f(m_Position.x + 200.0f - TILE_SIZE/2, m_Position.y - 1.0f - TILE_SIZE/2), sf::Color::Red)
        };
        Core()->Window()->draw(line, 2, sf::Lines);

        sf::Text text;
        text.setString(CLocale::getString(RESOURCE_STR_PULSA_ENCIMA_PARA_DESTRUIRLA));
        text.setFont(Core()->getDefaultFont());
        text.setCharacterSize(20.0f);
        text.setOrigin(0, text.getLocalBounds().height);
        text.setPosition(sf::Vector2f(m_Position.x - TILE_SIZE/2, m_Position.y - 5.0f - TILE_SIZE/2));
        text.setColor(sf::Color::Cyan);
        Core()->Window()->draw(text);
    }
}
