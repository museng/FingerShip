/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_PROJECTILE
#define H_PROJECTILE
#include "entity.h"
#include <SFML/Graphics.hpp>

class CProjectile : public CEntity
{
public:
	CProjectile(int owner, sf::Vector2f pos, sf::Color color, sf::Vector2f dir, float vel, int mode = 0);
	~CProjectile();

	virtual void tick();
	virtual void destroy();

	int m_Owner;

private:
	sf::CircleShape m_VisibleChar;
	sf::RectangleShape m_CollisionChar;

	sf::Vector2f m_Dir;
	float m_Vel;
	sf::Color m_Color;
	sf::Vector2f m_PrevPos;
	int m_Mode;
};

#endif
