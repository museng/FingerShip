/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_TURRET
#define H_TURRET
#include "entity.h"
#include <SFML/Graphics.hpp>

class CTurret : public CEntity
{
public:
	CTurret(sf::Vector2f pos);
	~CTurret();

	virtual void tick();
	virtual void destroy();

private:
	long m_ShootTime;
	sf::Vector2f m_OrgPos;
	bool m_Alive;
	bool m_Shoot;
};

#endif
