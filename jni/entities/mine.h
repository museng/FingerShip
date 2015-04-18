/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_MINE
#define H_MINE
#include "entity.h"
#include <SFML/Graphics.hpp>

class CMine : public CEntity
{
public:
	CMine(sf::Vector2f pos);
	~CMine();

	virtual void tick();
	virtual void destroy();

private:
	long m_MoveTimer;
	sf::Vector2f m_OrgPos;
	float m_Offset;
};

#endif
