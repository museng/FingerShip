/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_CUBE
#define H_CUBE
#include "entity.h"
#include <SFML/Graphics.hpp>
#include "../engine/quad.h"

class CCube : public CEntity
{
public:
	CCube(sf::Vector2f pos);
	CCube(sf::Vector2f pos, sf::Vector2f dir, int sizel);
	~CCube();

	virtual void tick();
	virtual void destroy();

	Quad& getQuad() { return m_Quad; }

	int m_Health;

private:
	float m_RotDir;
	sf::Vector2f m_Dir;
	Quad m_Quad;
};

#endif
