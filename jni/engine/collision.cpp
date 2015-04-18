/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "collision.h"
#include "game_core.h"
#include "basic_functions.h"
#include "../entities/entity.h"

CCollision::CCollision()
{
	m_pMap = nullptr;
}
CCollision::~CCollision()
{
	reset();
}

void CCollision::init(CMap *pMap)
{
	reset();
	m_pMap = pMap;
}

void CCollision::reset()
{
	if (m_pMap != nullptr)
		delete m_pMap;

	m_pMap = nullptr;
}

CTile* CCollision::isTileSolid(sf::Vector2f pos)
{
	if (!m_pMap)
		return 0x0;

	CTile *pTile = m_pMap->getTileAt(pos);
	if (pTile && pTile->m_Index&CTile::SOLID)
		return pTile;

	return 0x0;
}

CTile* CCollision::intersectLineTileSolid(sf::Vector2f ipos, sf::Vector2f epos, sf::Vector2f *rpos)
{
	// Fix: Map displacement
	ipos.y += TILE_SIZE*2 + (int)m_pMap->m_OffsetY%TILE_SIZE;
	epos.y += TILE_SIZE*2 + (int)m_pMap->m_OffsetY%TILE_SIZE;


	sf::Vector2f dir = epos - ipos;
	int length = vector_length(dir)+1;
	dir = vector_normalize(dir);

	sf::Vector2f tmpPos, tmpLPos;
	CTile *pTile;

	tmpPos = tmpLPos = ipos;
	int i=0;
	do
	{
		tmpPos += dir;
		if ((pTile = isTileSolid(tmpPos)))
		{
			if (rpos)
			{
				*rpos = tmpLPos;
				rpos->y -= TILE_SIZE*2 + (int)m_pMap->m_OffsetY%TILE_SIZE; // Fix: Map displacement
			}

			return pTile;
		}

		tmpLPos = tmpPos;
		i++;
	} while (i<length);

	if (rpos)
	{
		*rpos = epos;
		rpos->y -= TILE_SIZE*2 + (int)m_pMap->m_OffsetY%TILE_SIZE; // Fix: Map displacement
	}

	return 0x0;
}

CEntity* CCollision::isInEntity(sf::FloatRect rectI, int type)
{
	CGameCore *pCore = CGameCore::getInstance();

	std::list<CEntity*>::iterator it = pCore->getScreen()->getEntities()->begin();
	while (it != pCore->getScreen()->getEntities()->end())
	{
		if ((*it)->toDelete() || (*it)->getType() == CEntity::PROJECTILE || (type != -1 && (*it)->getType() != type))
		{
			++it;
			continue;
		}

		sf::FloatRect rect = (*it)->getCollChar().getGlobalBounds();
		if (rect.intersects(rectI))
			return (*it);

		++it;
	}

	return 0x0;
}
