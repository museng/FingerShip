/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "cube.h"
#include "../engine/basic_functions.h"
#include "../engine/game_core.h"
#include "../engine/locale.h"
#include "../engine/effects.h"
#include "../engine/quad.h"
#include <cmath>

CCube::CCube(sf::Vector2f pos)
: CEntity(CEntity::CUBE, pos, CTextureManager::TEXTURE_CUBE)
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
	m_Quad.setSize(size.x, size.y);

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

	if (m_Position.y > RSIZE_H + m_Quad.getLocalBounds().height || m_Position.x+m_Quad.getLocalBounds().width/2 < 0 || m_Position.x-m_Quad.getLocalBounds().width/2 > RSIZE_W)
	{
		setToDelete();
		return;
	}
	if (m_Health <= 0)
		destroy();

	m_Quad.setPosition(m_Position);

	if (!Core()->isPaused())
		m_Quad.setRotation(m_Quad.getRotation()+m_RotDir);

	//Core()->Window()->draw(m_CollChar);
	Core()->Window()->draw(m_Quad);
}
