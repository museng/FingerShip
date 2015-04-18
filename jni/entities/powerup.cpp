/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "powerup.h"
#include "../engine/game_core.h"
#include "../engine/effects.h"
#include <cmath>

CPowerup::CPowerup(sf::Vector2f pos, int type)
: CEntity(CEntity::POWERUP)
{
	m_Position = pos;
	m_Type = type;
	m_MoveTimer = 0L;

	sf::Vector2f size(TILE_SIZE/2, TILE_SIZE/2);
	m_CollChar = sf::RectangleShape(size);
	m_CollChar.setOrigin(sf::Vector2f(size.x/2, size.y/2));

	if (m_Type == SHIELD)
		m_CollChar.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_POWERUP_SHIELD));
	else if (m_Type == BOLT)
		m_CollChar.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_POWERUP_BOLT));
	else if (m_Type == STAR)
		m_CollChar.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_POWERUP_STAR));
	else if (m_Type == BAD_BOLT)
		m_CollChar.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_POWERUP_BAD_BOLT));
}
CPowerup::~CPowerup()
{ }

void CPowerup::destroy()
{
	Core()->SoundManager()->play(CSoundManager::SOUND_DESTROY);

	if (m_Type == BAD_BOLT)
		CEffects::createTrashSpark(m_Position, sf::Color::Red);
	else if (m_Type == SHIELD)
		CEffects::createTrashSpark(m_Position, sf::Color::Green);

	CEntity::destroy();
}


void CPowerup::tick()
{
	m_Position.y += Core()->getGameSpeed();
	if (m_Position.y > RSIZE_H + TILE_SIZE)
	{
		setToDelete();
		return;
	}

	// Render Character Init
	sf::Vector2f posPowerup = sf::Vector2f(m_Position.x + sinf(m_MoveTimer*0.05f)*12.0f, m_Position.y + cosf(m_MoveTimer*0.05f)*12.0f);
	m_CollChar.setPosition(posPowerup);

	Core()->Window()->draw(m_CollChar);

	if (!Core()->isPaused())
		m_MoveTimer++;
}
