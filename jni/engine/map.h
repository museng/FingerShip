/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_MAP
#define H_MAP
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include "game_core.h"


class CTile
{
public:
	enum
	{
		AIR = 1,
		SOLID = 2,
		DESTRUCTIBLE = 4
	};

	CTile()
	{
		m_Index = AIR;
		m_TexIndex = 0;
	}
	CTile(int index)
	{
		m_Index = index;
		m_TexIndex = 0;
	}

	int m_Index;
	int m_TexIndex;
};

class CGroupTile
{
public:
	enum
	{
		GROUP_TILE_BACKGROUND=0,
		GROUP_TILE_GAME,
		GROUP_TILE_FOREGROUND
	};

	CTile *m_pTiles;
	int m_Type;
};

class CMap
{
public:
	enum
	{
		RUN = 0,
		PAUSE,
		END,

		ZONE_A=0,
		ZONE_B,
		ZONE_C,
		ZONE_D,
	};

	CMap();
	~CMap();

	int m_Width;
	int m_Height;
	float m_OffsetX;
	float m_OffsetY;
	std::list<CGroupTile> m_GroupTiles;
	sf::Clock m_RenderTime;
	sf::Clock m_SpeedTime;
	float m_Speed;

	void setStatus(int status) { m_Status = status; }
	int getZone() const { return m_Zone; }

	void render();
	CTile* getTileAt(sf::Vector2f);

private:
	class CGameCore *m_pCore;

	void genMapZone();

	void renderMap();
	void renderGrid();

	int m_IndexGroupTileGame;
	sf::VertexArray m_Tilemap;

	sf::Clock m_TimerEnemyGen;
	bool m_GenEnemyShips;
	sf::Vector2f m_GenEnemyPos;
	int m_CountEnemyShips;
	int m_GenDir;
	int m_WayPosW;

	int m_Status;
	int m_Zone;
	bool m_BossCreated;
};

#endif
