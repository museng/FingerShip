/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_POWERUP
#define H_POWERUP
#include "entity.h"
#include <SFML/Graphics.hpp>

class CPowerup : public CEntity
{
public:
	enum
	{
		SHIELD = 0,
		BOLT,
		STAR,

		BAD_BOLT,
	};

	CPowerup(sf::Vector2f pos, int type);
	~CPowerup();

	virtual void tick();
	virtual void destroy();

	int getType() const { return m_Type; }

private:
	long m_MoveTimer;
	sf::Vector2f m_OrgPos;
	int m_Type;
};

#endif
