/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_COLLISION
#define H_COLLISION
#include "map.h"
#include "quad.h"
#include "../entities/entity.h"

class CCollision
{
public:
	CCollision();
	~CCollision();

	class CMap*	Map() { return m_pMap; }

	void init(class CMap *pMap);
	class CTile* isTileSolid(sf::Vector2f pos);
	class CTile* intersectLineTileSolid(sf::Vector2f ipos, sf::Vector2f epos, sf::Vector2f *rpos = 0x0);
	CEntity* isInEntity(sf::VertexArray &pointsA, int type = -1);
	bool isVertexArrayIntersecting(sf::VertexArray &a, sf::VertexArray &b);

	void reset();

private:
	class CMap *m_pMap;
};

#endif
