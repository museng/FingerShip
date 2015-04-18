/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "entity.h"
#include "../engine/game_core.h"

CEntity::CEntity(int type)
{
	m_pCore = CGameCore::getInstance();

	m_Type = type;
	m_ToDelete = false;
}
CEntity::~CEntity()
{ }

int CEntity::getType()
{
	return m_Type;
}

void CEntity::setPosition(sf::Vector2f pos)
{
	m_Position = pos;
}
sf::Vector2f const CEntity::getPosition()
{
	return m_Position;
}

void CEntity::destroy()
{
	setToDelete();
}
