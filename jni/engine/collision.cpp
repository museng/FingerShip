/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "collision.h"
#include "game_core.h"
#include "basic_functions.h"
#include "../entities/entity.h"
#include "../entities/cube.h"

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
	if (pTile && (pTile->m_Index&CTile::SOLID))
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

CEntity* CCollision::isInEntity(sf::VertexArray &pointsA, int type)
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

		if (isVertexArrayIntersecting(pointsA, (*it)->getQuad().getPoints()))
			return (*it);

		++it;
	}

	return 0x0;
}

bool CCollision::isVertexArrayIntersecting(sf::VertexArray &a, sf::VertexArray &b)
{
	sf::VertexArray quads[2] = { a, b };

	for (unsigned u = 0; u<2; u++)
	{
        for (int i1 = 0; i1 < quads[u].getVertexCount(); i1++)
        {
        	int i2 = (i1 + 1) % quads[u].getVertexCount();
        	sf::Vector2f p1 = quads[u][i1].position;
        	sf::Vector2f p2 = quads[u][i2].position;

        	sf::Vector2f normal(p2.y - p1.y, p1.x - p2.x);
        	float rval = 0.0f;

        	rval = normal.x * a[0].position.x + normal.y * a[0].position.y;
            float minA = rval, maxA = rval;
            for (unsigned q = 0; q < a.getVertexCount(); q++)
            {
            	sf::Vector2f p = a[q].position;
                float projected = normal.x * p.x + normal.y * p.y;
                if (projected < minA)
                    minA = projected;
                if (projected > maxA)
                    maxA = projected;
            }

            rval = normal.x * b[0].position.x + normal.y * b[0].position.y;
            float minB = rval, maxB = rval;
            for (unsigned q = 0; q < b.getVertexCount(); q++)
            {
            	sf::Vector2f p = b[q].position;
                float projected = normal.x * p.x + normal.y * p.y;
                if (projected < minA)
                    minB = projected;
                if (projected > maxA)
                    maxB = projected;
            }

            if (maxA < minB || maxB < minA)
                return false;
        }
	}

    return true;
}
