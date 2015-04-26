/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_BOSS_SPACESTATION
#define H_BOSS_SPACESTATION
#include "entity.h"
#include <SFML/Graphics.hpp>

class CBossSpaceStation : public CEntity
{
public:
	CBossSpaceStation(sf::Vector2f pos);
	~CBossSpaceStation();

	virtual void tick();
	virtual void destroy();

	int m_Health;

protected:
	long m_Timer;
	long m_ShootTimer;
	long m_DeadAnimTimer;

	sf::VertexArray m_VisibleChar;
	int m_Shoots;
	bool m_Dead;
	int m_ShootTime;

private:
	enum
	{
		NORMAL_ATTACK=0,
		HARD_ATTACK,
	};

	int m_RotDir;
	float m_Offset;
	int m_Status;
	bool m_ToUp;
	long m_StatusTime;
	float m_Rotation;

	Quad m_VisibleQuad; // TODO: Don't use this! use ArrayVertex for collision detection!
};

#endif
