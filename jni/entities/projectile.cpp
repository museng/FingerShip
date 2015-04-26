/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "projectile.h"
#include "../engine/game_core.h"
#include "../engine/effects.h"
#include "cube.h"
#include "meteor.h"
#include "boss_spacestation.h"

CProjectile::CProjectile(int owner, sf::Vector2f pos, sf::Color color, sf::Vector2f dir, float vel, int mode)
: CEntity(CEntity::PROJECTILE)
{
	m_Owner = owner;
	m_PrevPos = m_Position = pos;
	m_Dir = dir;
	m_Vel = vel;
	m_Color = color;
	m_Mode = mode;

	if (m_Owner == CEntity::TURRET)
	{
		sf::Vector2f size(9, 9);
		m_CollChar = sf::RectangleShape(sf::Vector2f(size.x, size.y));
		m_CollChar.setOrigin(sf::Vector2f(size.x/2, size.y/2));
		m_CollChar.setRotation(-vector_angle(dir));

		m_CollChar.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_BULLET_TURRET));
	}
	else if (m_Owner == CEntity::SPACESTATION)
	{
		sf::Vector2f size(9, 9);
		m_CollChar = sf::RectangleShape(sf::Vector2f(size.x, size.y));
		m_CollChar.setOrigin(sf::Vector2f(size.x/2, size.y/2));
		m_CollChar.setRotation(-vector_angle(dir));

		m_CollChar.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_BULLET_SPACESTATION));
	}
	else
	{
		sf::Vector2f size(22, 22);
		m_CollChar = sf::RectangleShape(sf::Vector2f(size.x, size.y));
		m_CollChar.setOrigin(sf::Vector2f(size.x/2, size.y/2));

		m_CollChar.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_BULLET));
	}
}
CProjectile::~CProjectile()
{ }

void CProjectile::destroy()
{
	CEffects::createBulletSpark(m_Position);
	Core()->SoundManager()->play(CSoundManager::SOUND_EXPLOSION);

	CEntity::destroy();
}


void CProjectile::tick()
{
	if (!Core()->Collision()->Map() || m_Position.x < 0-TILE_SIZE || m_Position.x > RSIZE_W+TILE_SIZE || m_Position.y < 0-TILE_SIZE || m_Position.y > RSIZE_H+TILE_SIZE)
	{
		setToDelete();
		return;
	}

	if (!Core()->isPaused())
	{
		m_Position.x += m_Dir.x*m_Vel;
		m_Position.y += m_Dir.y*m_Vel;
	}
	m_CollChar.setPosition(m_Position.x, m_Position.y);

	if (m_Owner == CEntity::CHARACTER)
	{
		//m_CollChar.setFillColor(m_Color);

		CEntity *pEntColl = Core()->Collision()->isInEntity(m_CollChar.getGlobalBounds());
		sf::Vector2f rPos;
		CTile *pTile = Core()->Collision()->intersectLineTileSolid(m_PrevPos, m_Position, &rPos);

		if (pTile)
		{
			if (pTile->m_Index&CTile::DESTRUCTIBLE)
			{
				int x = rPos.x/TILE_SIZE;
				int y = rPos.y/TILE_SIZE;

				pTile->m_Index = CTile::AIR;
				CEffects::createTrashSpark(sf::Vector2f(x*TILE_SIZE + TILE_SIZE/2, y*TILE_SIZE - TILE_SIZE/2), sf::Color(167, 126, 64));
			}

			CEffects::createBulletSpark(rPos);
			Core()->SoundManager()->play(CSoundManager::SOUND_EXPLOSION);
			setToDelete();
			return;
		}
		else if (pEntColl && pEntColl->getType() != CEntity::CHARACTER)
		{
			if (Core()->Player()->m_pCharacter)
			{
				if (pEntColl->getType() == CEntity::MINE)
				{
					CEffects::createPoints(pEntColl->getPosition(), 125);
					Core()->Player()->addPoints(125);
					Core()->m_MineKilled = true;
				} else if (pEntColl->getType() == CEntity::ENEMY_SHIP_A)
				{
					CEffects::createPoints(pEntColl->getPosition(), 75);
					Core()->Player()->addPoints(75);
					Core()->m_EnemyShipKilled = true;
				} else if (pEntColl->getType() == CEntity::METEOR)
				{
					CEffects::createPoints(pEntColl->getPosition(), 25);
					Core()->Player()->addPoints(25);
					Core()->m_AsteroidKilled = true;
				} else if (pEntColl->getType() == CEntity::DRON_LASER)
				{
					CEffects::createPoints(pEntColl->getPosition(), 50);
					Core()->Player()->addPoints(50);
					Core()->m_DronLaserKilled = true;
				} else if (pEntColl->getType() == CEntity::SPACEMAN)
				{
					CEffects::createPoints(pEntColl->getPosition(), -750);
					Core()->Player()->addPoints(-750);
				}
			}

			if (pEntColl->getType() == CEntity::METEOR)
			{
				sf::Vector2f tmpPos = sf::Vector2f(pEntColl->getPosition().x+pEntColl->getCollChar().getSize().x/2, pEntColl->getPosition().y+pEntColl->getCollChar().getSize().y/2);
				Core()->getScreen()->addEntity(new CMeteor(tmpPos, sf::Vector2f(0.25f, 1.0f), pEntColl->getCollChar().getSize().x));
				Core()->getScreen()->addEntity(new CMeteor(tmpPos, sf::Vector2f(-0.25f, 1.0f), pEntColl->getCollChar().getSize().x));
			}
			else if (pEntColl->getType() == CEntity::CUBE)
			{
				CCube *pCube = static_cast<CCube*>(pEntColl);
				pCube->m_Health--;

				CEffects::createDamageCube(pCube);
			}
			else if (pEntColl->getType() == CEntity::SPACESTATION)
			{
				CBossSpaceStation *pBoss = static_cast<CBossSpaceStation*>(pEntColl);
				if (Core()->Player()->m_pCharacter)
					pBoss->m_Health--;

				CEffects::createDamageBossSpaceStation(pBoss);
				CEffects::createExplosionSmall(m_Position, false);
				Core()->SoundManager()->play(CSoundManager::SOUND_BOSS_DAMAGE);
			}

			if (pEntColl->getType() != CEntity::CUBE && pEntColl->getType() != CEntity::SPACESTATION)
				pEntColl->destroy();

			destroy();
		}
		else if (!Core()->isPaused())
			CEffects::createCharacterProjectileTrail(m_Position);
	} else if (m_Owner == CEntity::TURRET)
	{
		if (Core()->Player()->m_pCharacter && Core()->Player()->m_pCharacter->getCollChar().getGlobalBounds().contains(sf::Vector2f(m_Position.x, m_Position.y)))
		{
			if (!Core()->Player()->m_pCharacter->hasShield())
				Core()->Player()->destroyCharacter(true);
			else
				Core()->Player()->m_pCharacter->dropShield();

			destroy();
		}
		else if (!Core()->isPaused())
			CEffects::createTurretProjectileTrail(m_Position);
	} else if (m_Owner == CEntity::SPACESTATION)
	{
		if (Core()->Player()->m_pCharacter && Core()->Player()->m_pCharacter->getCollChar().getGlobalBounds().contains(sf::Vector2f(m_Position.x, m_Position.y)))
		{
			if (!Core()->Player()->m_pCharacter->hasShield())
				Core()->Player()->destroyCharacter(true);
			else
				Core()->Player()->m_pCharacter->dropShield();

			destroy();
		}
	}

	Core()->Window()->draw(m_CollChar, sf::BlendAdd);
	m_PrevPos = m_Position;
}
