/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_ENTITY
#define H_ENTITY

#include "../engine/quad.h"
#include <SFML/Graphics.hpp>

class CEntity
{
public:
	class CGameCore *Core() { return m_pCore; }

	enum
	{
		CHARACTER=1,
		MINE,
		PROJECTILE,
		TURRET,
		ENEMY_SHIP_A,
		METEOR,
		DRON_LASER,
		SPACEMAN,
		SPACESTATION,
		POWERUP,
		CUBE,
		NUM_ENTITIES
	};

	CEntity(int type, sf::Vector2f pos, int textureId);
	CEntity(int type, sf::Vector2f pos, float width, float height, int textureId);
	virtual ~CEntity();

	virtual void tick() = 0;
	virtual void destroy() = 0;

	int getType();

	void setPosition(sf::Vector2f pos);
	sf::Vector2f const getPosition();

	void setToDelete() { m_ToDelete = true; }
	bool toDelete() const { return m_ToDelete; }

	Quad& getQuad() { return m_Quad; }

protected:
	sf::Vector2f m_Position;
	Quad m_Quad;

private:
	class CGameCore *m_pCore;
	int m_Type;
	bool m_ToDelete;
};

#endif
