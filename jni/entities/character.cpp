/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "character.h"
#include "projectile.h"
#include "../engine/particle.h"
#include "../engine/effects.h"

int CCharacter::SIZE = 52;
CCharacter::CCharacter()
: CEntity(CEntity::CHARACTER)
{
	m_CanShoot = true;
	m_OverHeat = 0.0f;
	m_OverHeating = false;
	m_Shield = false;
	m_Move = false;

	sf::Vector2f size(SIZE, SIZE);
	m_VisibleChar = sf::CircleShape(size.x); // Radio
	m_VisibleChar.setOrigin(sf::Vector2f(size.x, size.y));
	m_VisibleChar.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_SHIP));

	m_CollChar = sf::RectangleShape(sf::Vector2f(size.x/1.5f, size.y/1.5f));
	m_CollChar.setOrigin(sf::Vector2f(size.x/2, size.y/2));

	int offset = TILE_SIZE*USER_ZONE_LIMIT_DOWN - TILE_SIZE*USER_ZONE_LIMIT_UP;
	offset = TILE_SIZE*USER_ZONE_LIMIT_UP + offset/2;

	m_PrevPos = m_Position = sf::Vector2f(RSIZE_W/2, offset);

	m_VisibleChar.setPosition(m_Position.x, m_Position.y);
	m_CollChar.setPosition(m_Position.x, m_Position.y);
}
CCharacter::~CCharacter()
{ }

void CCharacter::destroy()
{
	CEffects::createExplosion(m_Position, false);
	Core()->SoundManager()->play(CSoundManager::SOUND_DESTROY);

	CEntity::destroy();
}

void CCharacter::reset()
{
	m_CanShoot = true;
	m_Move = false;
}

void CCharacter::tick()
{
	if (m_Position.x < 0) m_Position.x = 0;
	if (m_Position.x > RSIZE_W) m_Position.x = RSIZE_W;
	if (m_Position.y < 0) m_Position.y = 0;
	if (m_Position.y > RSIZE_H) m_Position.y = RSIZE_H;

	if (!Core()->isPaused())
	{
		if (sf::Touch::isDown(0) && Core()->getScreen()->m_InputActive)
		{
			sf::Vector2f convCoords = Core()->Window()->mapPixelToCoords(sf::Touch::getPosition(0));
			if (m_VisibleChar.getGlobalBounds().contains(convCoords))
			{
				m_Move = true;

				if (m_CanShoot)
				{
					m_CanShoot = false;
					if (!m_OverHeating)
					{
						Core()->getScreen()->addEntity(new CProjectile(getType(), m_Position, sf::Color::Red, sf::Vector2f(0.0f, -1.0f), 10.0f));
						Core()->SoundManager()->play(CSoundManager::SOUND_SHOOT);
						m_OverHeat += 5.0f;
						if (m_OverHeat > 100.0f)
						{
							m_OverHeat = 100.0f;
							m_OverHeating = true;
						}
					}
					else
						Core()->SoundManager()->play(CSoundManager::SOUND_NO_AMMO);
				}
			}

			if (m_Move)
			{
				m_Position = sf::Vector2f(convCoords.x, convCoords.y);
				if (m_Position.y > RSIZE_H-100) m_Position.y = RSIZE_H-100; // Fix: Android 5.x buttons zone
			}
		} else
		{
			m_CanShoot = true;
			m_Move = false;
		}

		// Enfriar Arma
		if (m_Timer.getElapsedTime().asSeconds() > 0.25f)
		{
			m_OverHeat -= (m_OverHeating)?8:1;
			if (m_OverHeat < 0)
			{
				m_OverHeat = 0;
				m_OverHeating = false;
			}
			m_Timer.restart();
		}

		m_VisibleChar.setPosition(m_Position.x, m_Position.y);
		m_CollChar.setPosition(m_Position.x, m_Position.y);

		/*int x = m_Position.x/TILE_SIZE;
		int y = m_Position.y/TILE_SIZE;
		sf::RectangleShape rectShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		rectShape.setFillColor(sf::Color::Blue);
		rectShape.setPosition(x*TILE_SIZE + g_Core.m_pMap->m_OffsetX%TILE_SIZE, y*TILE_SIZE + g_Core.m_pMap->m_OffsetY%TILE_SIZE);
		g_Core.getWindow()->draw(rectShape);*/

		CEffects::createFireTrail(m_Position);
		CEffects::createLightRedTrail(m_Position);
	}

	// Render Shield
	if (m_Shield)
	{
		sf::RectangleShape rectShield = sf::RectangleShape(sf::Vector2f(144, 137));
		rectShield.setOrigin(sf::Vector2f(144/2, 137/2));
		rectShield.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_SHIP_SHIELD));
		rectShield.setPosition(m_Position);
		Core()->Window()->draw(rectShield);
	}

	Core()->Window()->draw(m_VisibleChar);
	m_PrevPos = m_Position;
}

void CCharacter::giveShield()
{
	m_Shield = true;
	Core()->SoundManager()->play(CSoundManager::SOUND_SHIELD_UP);
}
void CCharacter::dropShield()
{
	m_Shield = false;
	Core()->SoundManager()->play(CSoundManager::SOUND_SHIELD_DOWN);
}
