/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_DRON_LASER
#define H_DRON_LASER
#include "entity.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class CDronLaser : public CEntity
{
public:
	CDronLaser(sf::Vector2f pos);
	~CDronLaser();

	virtual void tick();
	virtual void destroy();

private:
	sf::Clock m_Timer;
	sf::Clock m_TimerShoot;
	sf::Vector2f m_OrgPos;
	bool m_Active;
	bool m_Shoot;
	bool m_ShootSound;

	float m_LaserEnergy;
	sf::Sound *m_pSoundLaserA;
};

#endif
