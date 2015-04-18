/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_ENEMY_SHIP_A
#define H_ENEMY_SHIP_A
#include "entity.h"
#include <SFML/Graphics.hpp>

class CEnemyShipA : public CEntity
{
public:
	CEnemyShipA(sf::Vector2f pos, float offset);
	~CEnemyShipA();

	virtual void tick();
	virtual void destroy();

private:
	long m_MoveTimer;
	sf::Vector2f m_OrgPos;
	float m_Offset;
};

#endif
