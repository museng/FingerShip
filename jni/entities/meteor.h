/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_METEOR
#define H_METEOR
#include "entity.h"
#include <SFML/Graphics.hpp>

class CMeteor : public CEntity
{
public:
	CMeteor(sf::Vector2f pos);
	CMeteor(sf::Vector2f pos, sf::Vector2f dir, int sizel);
	~CMeteor();

	virtual void tick();
	virtual void destroy();

private:
	int m_RotDir;
	int m_Vel;
	sf::Vector2f m_Dir;
};

#endif
