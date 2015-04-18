/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_CHARACTER
#define H_CHARACTER
#include "entity.h"
#include <SFML/Graphics.hpp>

class CCharacter : public CEntity
{
public:
	static int SIZE;

	CCharacter();
	~CCharacter();

	virtual void tick();
	virtual void destroy();

	void reset();

	sf::Vector2f getPrevPosition() { return m_PrevPos; }

	float m_OverHeat;
	bool m_OverHeating;

	void giveShield();
	void dropShield();
	bool hasShield() const { return m_Shield; }

private:
	sf::CircleShape m_VisibleChar;
	sf::RectangleShape m_CollisionChar;
	sf::Clock m_Timer;
	sf::Vector2f m_PrevPos;

	bool m_Shield;
	bool m_CanShoot;
	bool m_Move;
};

#endif
