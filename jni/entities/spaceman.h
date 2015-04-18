/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_SPACEMAN
#define H_SPACEMAN
#include "entity.h"
#include <SFML/Graphics.hpp>

class CSpaceman: public CEntity
{
public:
	CSpaceman(sf::Vector2f pos);
	~CSpaceman();

	virtual void tick();
	virtual void destroy();

private:
	int m_RotDir;
	int m_Vel;
	sf::Vector2f m_Dir;

	long m_AnimTimer;
	int m_AnimIndex;
};

#endif
