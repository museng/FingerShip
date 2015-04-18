/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "cube.h"
#include "../engine/basic_functions.h"
#include "../engine/game_core.h"
#include "../engine/locale.h"
#include "../engine/effects.h"
#include <cmath>

CCube::CCube(sf::Vector2f pos)
: CEntity(CEntity::CUBE)
{
	m_Position = pos;
	m_Health = 6;

	sf::Vector2f rawDir(0.0f, 1.0f);
	if (Core()->Player()->m_pCharacter)
		rawDir = Core()->Player()->m_pCharacter->getPosition() - m_Position;
	m_Dir = sf::Vector2f(rawDir.x, rawDir.y);
	m_Dir = vector_normalize(m_Dir);

	int rand = random_int(TILE_SIZE, TILE_SIZE*3);
	sf::Vector2f size(rand, rand);
	m_CollChar = sf::RectangleShape(size);
	m_CollChar.setOrigin(sf::Vector2f(size.x/2, size.y/2));
	m_CollChar.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_CUBE));
	m_CollChar.setPosition(m_Position);

	m_RotDir = random_float(-0.25, 0.25);
}
CCube::~CCube()
{ }

void CCube::destroy()
{
	CEffects::createExplosion(m_Position, true);
	Core()->SoundManager()->play(CSoundManager::SOUND_DESTROY);

	CEntity::destroy();
}


void CCube::tick()
{
	m_Position.y += Core()->getGameSpeed();

	if (m_Position.y > RSIZE_H + m_CollChar.getSize().y || m_Position.x+m_CollChar.getLocalBounds().width/2 < 0 || m_Position.x-m_CollChar.getLocalBounds().width/2 > RSIZE_W)
	{
		setToDelete();
		return;
	}
	if (m_Health <= 0)
		destroy();

	m_CollChar.setPosition(m_Position);

	if (!Core()->isPaused())
		m_CollChar.setRotation(m_CollChar.getRotation()+m_RotDir);

	Core()->Window()->draw(m_CollChar);
}
