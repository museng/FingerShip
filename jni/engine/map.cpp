/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "map.h"
#include "basic_functions.h"
#include "particle.h"
#include "../entities/mine.h"
#include "../entities/turret.h"
#include "../entities/enemy_ship.h"
#include "../entities/meteor.h"
#include "../entities/dron_laser.h"
#include "../entities/spaceman.h"
#include "../entities/powerup.h"
#include "../entities/cube.h"
#include "../entities/boss_spacestation.h"

CMap::CMap()
:
 m_Tilemap(sf::Triangles, SIZE_W*(SIZE_H+TILES_MARGIN*2)*3*2)
{
	metersToZone[ZONE_A] = 0.0f;
	metersToZone[ZONE_B] = 5.0f;
	metersToZone[ZONE_C] = 9.0f;
	metersToZone[ZONE_D] = MAX_METERS;

	m_pCore = CGameCore::getInstance();
	m_Status = RUN;
	m_Zone = ZONE_A;
	m_BossCreated = false;
	m_NumTilesRendered = 0;

	m_Width = RSIZE_W/64;
	m_Height = 1;
	m_OffsetX = m_OffsetY = 0.0f;
	m_IndexGroupTileGame = -1;
	m_Speed = 5;
	m_GenEnemyShips = false;
	m_CountEnemyShips = 0;

	CGroupTile groupTile;
	groupTile.m_Type == CGroupTile::GROUP_TILE_GAME;
	groupTile.m_pTiles = (CTile*)malloc(sizeof(CTile) * SIZE_W * SIZE_H);
	for (int i=0; i<SIZE_W*SIZE_H; groupTile.m_pTiles[i++].m_Index = CTile::AIR);

	m_GroupTiles.push_back(groupTile);
	m_IndexGroupTileGame = 0;
	m_GenDir = 0;
	m_WayPosW = m_Width/2;

	srand(581987);
	setZone(ZONE_A);
}
CMap::~CMap()
{
	std::list<CGroupTile>::iterator it = m_GroupTiles.begin();
	while (it != m_GroupTiles.end())
	{
		free((*it).m_pTiles);
		++it;
	}

	m_GroupTiles.clear();
}

void CMap::render()
{
	if (!m_pCore->isPaused())
	{
		if (m_Speed < 16.0f && m_Status == RUN && m_SpeedTime.getElapsedTime().asMilliseconds() > 4500)
		{
			m_Speed += 0.1f;
			m_SpeedTime.restart();
		}
		if (m_Zone == ZONE_D && m_Speed < 28.0f && m_SpeedTime.getElapsedTime().asMilliseconds() > 125)
		{
			m_Speed += 0.25f;
			m_SpeedTime.restart();
		}

		m_OffsetY += m_Speed;
	}

	// Check Zone
	const float metersDisplaced = m_OffsetY/PIXELS_IN_METER;
	if (metersDisplaced >= metersToZone[m_Zone+1])
		setZone(m_Zone+1);

	genMapZone();
	renderMap();

	if (m_pCore->m_Config.m_Debug)
		renderGrid();
}

void CMap::genMapZone()
{
	int cols = m_OffsetY/TILE_SIZE+TILES_MARGIN;
	if (cols > m_Height)
	{
		float metersDisplaced = m_OffsetY/PIXELS_IN_METER;
		std::list<CGroupTile>::iterator it = m_GroupTiles.begin();
		std::advance(it, m_IndexGroupTileGame);

		int nums = cols - m_Height;
		m_Height = cols;
		int maxH = (m_Height < SIZE_H+TILES_MARGIN*2)?m_Height:SIZE_H+TILES_MARGIN*2;
		CTile *pNewTiles = (CTile*)malloc(sizeof(CTile) * SIZE_W*maxH);
		memset(pNewTiles, 0, sizeof(CTile) * SIZE_W*maxH);
		int vur = random_int(-1, 1);
		for (int i=0; i<maxH; i++)
		{
			if (m_Zone == ZONE_A || m_Zone == ZONE_C)
			{
				if (i<nums)
				{
					for (int e=0; e<SIZE_W; pNewTiles[i*m_Width+e++].m_Index = CTile::SOLID); // Clean Line

					m_GenDir = vur;
					m_WayPosW += m_GenDir;

					if (m_WayPosW >= m_Width) m_WayPosW = m_Width-1;
					else if (m_WayPosW < 0) m_WayPosW = 0;

					int e = m_WayPosW;
					int aleat = random_int(0, 50);

					for (int o=-1*SIZE_WAY; o <= SIZE_WAY; o++)
					{
						int randSpaceman = random_int(0, 290);
						int randMines = random_int(0, 90);

						if (e+o >= 0 && e+o < m_Width) // Dentro del camino
						{
							int index = i*m_Width+(e+o);

							// Way
							pNewTiles[index] = CTile(CTile::AIR);

							// Stars (INFO: FAKE EFFECT - GOOD FOR MOBILE)
							if (!m_pCore->m_Config.m_LowGraphics && vur+1 == 1)
							{
								int randStarts = random_int(1, (e+o)*TILE_SIZE+TILE_SIZE/2);
								CParticleStar *pStar = new CParticleStar();
								pStar->m_Pos = sf::Vector2f(randStarts, i*TILE_SIZE+TILE_SIZE/2);
								m_pCore->getScreen()->addParticle(pStar);
							}

							if (i == 0) // Dentro del camino pero solo en la primera fila (fuera de lo que ve el usuario)
							{
								// Enemies (MINES)
								if (randMines == 3 && m_pCore->getScreen()->getEntitiesByType(CEntity::MINE).size() < 4)
									m_pCore->getScreen()->addEntity(new CMine(sf::Vector2f((e+o)*TILE_SIZE + TILE_SIZE/2, i*TILE_SIZE - RTILES_MARGIN + TILE_SIZE/2)));

								// Powerup (SPACEMAN)
								if (randSpaceman == 15 && m_pCore->getScreen()->getEntities()->size() >= 3 && m_pCore->getScreen()->getEntitiesByType(CEntity::SPACEMAN).size() == 0)
									m_pCore->getScreen()->addEntity(new CSpaceman(sf::Vector2f((e+o)*TILE_SIZE + TILE_SIZE/2, i*TILE_SIZE - RTILES_MARGIN + TILE_SIZE/2)));

								// Enemies (DRON LASER)
								if (metersDisplaced > 2.0f && randMines == 8 && m_pCore->getScreen()->getEntitiesByType(CEntity::DRON_LASER).size() < 2 && m_pCore->getScreen()->getEntitiesByType(CEntity::DRON_LASER).size() < 3)
									m_pCore->getScreen()->addEntity(new CDronLaser(sf::Vector2f((e+o)*TILE_SIZE + TILE_SIZE/2, i*TILE_SIZE - RTILES_MARGIN + TILE_SIZE/2)));

								// Walls
								if (aleat == 5 && m_pCore->Player()->m_pCharacter && !m_pCore->Player()->m_pCharacter->m_OverHeating)
								{
									pNewTiles[index] = CTile(CTile::SOLID|CTile::DESTRUCTIBLE);
									pNewTiles[index].m_TexIndex = 15 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
								}

								// Powerup SHIELD
								if (randMines == 13 && m_pCore->Player()->m_pCharacter && !m_pCore->Player()->m_pCharacter->hasShield() && m_pCore->getScreen()->getEntitiesByType(CEntity::POWERUP).size() == 0)
									m_pCore->getScreen()->addEntity(new CPowerup(sf::Vector2f((e+o)*TILE_SIZE + TILE_SIZE/2, i*TILE_SIZE - RTILES_MARGIN + TILE_SIZE/2), CPowerup::SHIELD));
								// Powerup BAD BOLT
								if (metersDisplaced > 1.0f && randMines == 23 && m_pCore->getScreen()->getEntitiesByType(CEntity::POWERUP).size() < 5)
									m_pCore->getScreen()->addEntity(new CPowerup(sf::Vector2f((e+o)*TILE_SIZE + TILE_SIZE/2, i*TILE_SIZE - RTILES_MARGIN + TILE_SIZE/2), CPowerup::BAD_BOLT));
							}
						}
					}

					if (i == 0) // En cualquier parte del eje X pero solo en la primera fila (fuera de lo que ve el usuario)
					{
						int wr = random_int(0, SIZE_W-1); // Posicion Randmon
						int ur = random_int(0, 20); // Randon de alta probabilidad
						int lr = random_int(0, 100); // Random de baja probabilidad
						int ccr[] = { 5, 10, 8, 12 };


						// Turrets
						if (ur == ccr[0] && wr >= 0 && wr < SIZE_W-1
							&& (pNewTiles[i*m_Width+wr].m_Index&CTile::SOLID)
							&& (pNewTiles[i*m_Width+(wr-1)].m_Index&CTile::SOLID)
							&& (pNewTiles[i*m_Width+(wr+1)].m_Index&CTile::SOLID)
							&& m_pCore->getScreen()->getEntitiesByType(CEntity::TURRET).size() < 3)
						{
							pNewTiles[i*m_Width+wr].m_TexIndex = 16 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
							m_pCore->getScreen()->addEntity(new CTurret(sf::Vector2f(wr*TILE_SIZE + TILE_SIZE/2, i*TILE_SIZE - RTILES_MARGIN + TILE_SIZE/2)));
						}

						// Enemies (SHIP)
						static float shipOffset = 0.0f;
						if (!m_GenEnemyShips && metersDisplaced > 9.0f && lr == ccr[2])
						{
							m_GenEnemyShips = true;
							m_CountEnemyShips = 0;
							m_GenEnemyPos = sf::Vector2f(e*TILE_SIZE + TILE_SIZE/2, i*TILE_SIZE - RTILES_MARGIN + TILE_SIZE/2);
							shipOffset = random_float(-3.0f, 3.0f);
							if (shipOffset > -1.0f && shipOffset < 1.0f) shipOffset = 1.0f;
						}
						if (m_GenEnemyShips)
						{
							if (m_CountEnemyShips > 3) m_GenEnemyShips = false;
							else
							{
								m_CountEnemyShips++;
								m_pCore->getScreen()->addEntity(new CEnemyShipA(m_GenEnemyPos, shipOffset));
								m_TimerEnemyGen.restart();
							}
						}

						// Meteors
						if (metersDisplaced > 5.0f && ur == ccr[3])
							m_pCore->getScreen()->addEntity(new CMeteor(sf::Vector2f(wr*TILE_SIZE + TILE_SIZE/2, i*TILE_SIZE - RTILES_MARGIN + TILE_SIZE/2)));
					}
				} else
				{
					for (int e=0; e<m_Width; e++)
					{
						int index = i*m_Width+e;
						int tindex = (i-nums)*m_Width+e;
						pNewTiles[index] = (*it).m_pTiles[tindex];
					}
				}
			}
			else if (m_Zone == ZONE_B)
			{
				for (int e=0; e<SIZE_W; pNewTiles[i*m_Width+e++].m_Index = CTile::AIR); // Clean Line

				if (i == 0) // En cualquier parte del eje X pero solo en la primera fila (fuera de lo que ve el usuario)
				{
					for (int e=0; e<SIZE_W; e++)
					{
						int ra = random_int(0,30);
						if (ra == 3)
						{
							CCube *pCube = new CCube(sf::Vector2f(e*TILE_SIZE + TILE_SIZE/2, i*TILE_SIZE - RTILES_MARGIN + TILE_SIZE/2));

							if (!m_pCore->Collision()->isInEntity(pCube->getQuad().getPoints(), CEntity::CUBE))
								m_pCore->getScreen()->addEntity(pCube);
							else
								delete pCube;
						}
						else if (ra == 13)
						{
							int rsize = random_int(TILE_SIZE, TILE_SIZE*3);
							CParticle *pParticle = new CParticle(sf::BlendAlpha, RENDER_BACK);
							pParticle->m_Pos = sf::Vector2f(e*TILE_SIZE + TILE_SIZE/2, i*TILE_SIZE - RTILES_MARGIN + TILE_SIZE/2);
							pParticle->m_SizeInit = sf::Vector2f(rsize, rsize);
							pParticle->m_SizeEnd = sf::Vector2f(rsize, rsize);
							pParticle->m_ColorInit = sf::Color(40,40,40);
							pParticle->m_ColorEnd = sf::Color(40,40,40);
							pParticle->m_Duration = -1.0f;
							pParticle->m_VelRot = random_float(-0.25, 0.25);
							pParticle->m_Shape.setTexture(m_pCore->TextureManager()->get(CTextureManager::TEXTURE_CUBE));
							m_pCore->getScreen()->addParticle(pParticle);
						}

						// Stars
						if (!m_pCore->m_Config.m_LowGraphics)
						{
							int randStarts = random_int(-TILE_SIZE, RSIZE_W);
							CParticleStar *pStar = new CParticleStar();
							pStar->m_Pos = sf::Vector2f(randStarts, i*TILE_SIZE+TILE_SIZE/2);
							m_pCore->getScreen()->addParticle(pStar);
						}
					}
				}
				else if (i>=nums)
				{
					for (int e=0; e<m_Width; e++)
					{
						int index = i*m_Width+e;
						int tindex = (i-nums)*m_Width+e;
						pNewTiles[index] = (*it).m_pTiles[tindex];
					}
				}
			}
			else if (m_Zone == ZONE_D)
			{
				for (int e=0; e<SIZE_W; pNewTiles[i*m_Width+e++].m_Index = CTile::AIR); // Clean Line

				if (i == 0) // En cualquier parte del eje X pero solo en la primera fila (fuera de lo que ve el usuario)
				{
					if (!m_BossCreated)
					{
						m_pCore->getScreen()->addEntity(new CBossSpaceStation(sf::Vector2f(RSIZE_W/2, -RSIZE_H/2)));
						m_BossCreated = true;
					}

					for (int e=0; e<SIZE_W; e++)
					{
						// Stars
						if (!m_pCore->m_Config.m_LowGraphics)
						{
							int randStarts = random_int(-TILE_SIZE, RSIZE_W);
							CParticleStar *pStar = new CParticleStar();
							pStar->m_Pos = sf::Vector2f(randStarts, i*TILE_SIZE+TILE_SIZE/2);
							m_pCore->getScreen()->addParticle(pStar);
						}
					}
				}
				else if (i>=nums)
				{
					for (int e=0; e<m_Width; e++)
					{
						int index = i*m_Width+e;
						int tindex = (i-nums)*m_Width+e;
						pNewTiles[index] = (*it).m_pTiles[tindex];
					}
				}
			}
		}

		free((*it).m_pTiles);
		(*it).m_pTiles = pNewTiles;
	}

	// Extra Analysis
	const int normal_tile_index[] = {5, 11, 17};
	int maxH = (m_Height < SIZE_H+6)?m_Height:SIZE_H+6;
	std::list<CGroupTile>::iterator it = m_GroupTiles.begin();
	std::advance(it, m_IndexGroupTileGame);
	CTile *pTiles = (*it).m_pTiles;
	for (short i=0; i<TILES_MARGIN; i++) // Limit to 3 first (Current, Top, Bottom)
	{
		for (short e=0; e<m_Width; e++)
		{
			const short index = i*m_Width+e;
			const short indexL = i*m_Width+(e-1);
			const short indexR = i*m_Width+(e+1);
			const short indexT = (i-1)*m_Width+e;
			const short indexB = (i+1)*m_Width+e;
			const short indexLT = (i-1)*m_Width+(e-1);
			const short indexRT = (i-1)*m_Width+(e+1);
			const short indexLB = (i+1)*m_Width+(e-1);
			const short indexRB = (i+1)*m_Width+(e+1);

			// Smooth Map
			if (e-1 >= 0 && i-1 > 0 && i+1 <= maxH && (pTiles[index].m_Index&CTile::SOLID) && pTiles[indexT].m_Index == CTile::AIR && pTiles[indexB].m_Index == CTile::AIR && ((pTiles[indexL].m_Index&CTile::DESTRUCTIBLE) || (pTiles[indexR].m_Index&CTile::DESTRUCTIBLE)))
			{
				pTiles[index].m_Index = CTile::SOLID|CTile::DESTRUCTIBLE;
				pTiles[index].m_TexIndex = 15 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
			}
			else if (e-1 >= 0 && i-1 > 0 && i+1 <= maxH && (pTiles[index].m_Index&CTile::SOLID) && pTiles[indexT].m_Index == CTile::AIR && pTiles[indexB].m_Index == CTile::AIR && (pTiles[indexL].m_Index == CTile::AIR || pTiles[indexR].m_Index == CTile::AIR))
				pTiles[index].m_Index = CTile::AIR;

			// Texturize!
			if (pTiles[index].m_Index == CTile::SOLID)
			{
				// Normal
				if (pTiles[index].m_TexIndex == 0)
					pTiles[index].m_TexIndex = ((random_int(0,20) == 2)?normal_tile_index[random_int(0, 2)]:7) + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
				// Esquina Inferior Derecha
				if (e-1 >= 0 && i-1 > 0 && i+1 <= maxH && (pTiles[index].m_Index&CTile::SOLID) && pTiles[indexB].m_Index == CTile::AIR && pTiles[indexL].m_Index == CTile::AIR && (pTiles[indexT].m_Index&CTile::SOLID))
					pTiles[index].m_TexIndex = 12 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
				// Esquina Inferior Izquierda
				else if (e+1 < m_Width && i-1 >= 0 && i+1 <= maxH && (pTiles[index].m_Index&CTile::SOLID) && pTiles[indexB].m_Index == CTile::AIR && pTiles[indexR].m_Index == CTile::AIR && (pTiles[indexT].m_Index&CTile::SOLID))
					pTiles[index].m_TexIndex = 14 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
				// Esquina Superior Derecha
				else if (e-1 >= 0 && i-1 >= 0 && i+1 <= maxH && (pTiles[index].m_Index&CTile::SOLID) && pTiles[indexT].m_Index == CTile::AIR && pTiles[indexL].m_Index == CTile::AIR && (pTiles[indexB].m_Index&CTile::SOLID))
					pTiles[index].m_TexIndex = 1 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
				// Esquina Superior Izquierda
				else if (e+1 < m_Width && i-1 >= 0 && i+1 <= maxH && (pTiles[index].m_Index&CTile::SOLID) && pTiles[indexT].m_Index == CTile::AIR && pTiles[indexR].m_Index == CTile::AIR && (pTiles[indexB].m_Index&CTile::SOLID))
					pTiles[index].m_TexIndex = 2 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
				// Esquina Inferior Derecha FIX
				else if (e-1 >= 0 && i-1 >= 0 && i+1 <= maxH && (pTiles[index].m_Index&CTile::SOLID) && (pTiles[indexL].m_Index&CTile::SOLID) && (pTiles[indexB].m_Index&CTile::SOLID) && pTiles[indexLB].m_Index == CTile::AIR)
					pTiles[index].m_TexIndex = 9 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
				// Esquina Superior Derecha FIX
				else if (e-1 >= 0 && i-1 >= 0 && i+1 <= maxH && (pTiles[index].m_Index&CTile::SOLID) && (pTiles[indexL].m_Index&CTile::SOLID) && (pTiles[indexT].m_Index&CTile::SOLID) && pTiles[indexLT].m_Index == CTile::AIR)
					pTiles[index].m_TexIndex = 3 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
				// Esquina Inferior Izquierda FIX
				else if (e+1 < m_Width && i-1 >= 0 && i+1 <= maxH && (pTiles[index].m_Index&CTile::SOLID) && (pTiles[indexR].m_Index&CTile::SOLID) && (pTiles[indexB].m_Index&CTile::SOLID) && pTiles[indexRB].m_Index == CTile::AIR)
					pTiles[index].m_TexIndex = 10 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
				// Esquina Superior Izquierda FIX
				else if (e+1 < m_Width && i-1 >= 0 && i+1 <= maxH && (pTiles[index].m_Index&CTile::SOLID) && (pTiles[indexR].m_Index&CTile::SOLID) && (pTiles[indexT].m_Index&CTile::SOLID) && pTiles[indexRT].m_Index == CTile::AIR)
					pTiles[index].m_TexIndex = 4 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
				// Lado Izquierdo
				else if (e-1 >= 0 && pTiles[indexL].m_Index == CTile::AIR)
					pTiles[index].m_TexIndex = 6 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
				// Lado Derecho
				else if (e+1 < m_Width && pTiles[indexR].m_Index == CTile::AIR)
					pTiles[index].m_TexIndex = 8 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
				// Bajo
				else if (i+1 <= maxH && (pTiles[index].m_Index&CTile::SOLID) && pTiles[indexB].m_Index == CTile::AIR)
					pTiles[index].m_TexIndex = 13 + ((m_Zone==ZONE_A||m_Zone==ZONE_B)?0:18);
			}
		}

		if (i == 1 && (pTiles[i*m_Width].m_Index&CTile::SOLID) && pTiles[(i-1)*m_Width].m_Index == CTile::AIR && pTiles[(i+1)*m_Width].m_Index == CTile::AIR)
			pTiles[i*m_Width].m_Index = (pTiles[i*m_Width+1].m_Index&CTile::DESTRUCTIBLE)?CTile::SOLID|CTile::DESTRUCTIBLE:CTile::AIR;
		if (i == 1 && (pTiles[i*m_Width+(m_Width-1)].m_Index&CTile::SOLID) && pTiles[(i-1)*m_Width+(m_Width-1)].m_Index == CTile::AIR && pTiles[(i+1)*m_Width+(m_Width-1)].m_Index == CTile::AIR)
			pTiles[i*m_Width+(m_Width-1)].m_Index = (pTiles[i*m_Width+(m_Width-2)].m_Index&CTile::DESTRUCTIBLE)?CTile::SOLID|CTile::DESTRUCTIBLE:CTile::AIR;
	}
}

void CMap::renderMap()
{
	int maxH = (m_Height < SIZE_H+TILES_MARGIN*2)?m_Height:SIZE_H+TILES_MARGIN*2;
	m_NumTilesRendered = 0;

	std::list<CGroupTile>::iterator it = m_GroupTiles.begin();
	while (it != m_GroupTiles.end())
	{
		CTile *pTiles = (*it).m_pTiles;
		const int tsize = 48;

		for (short i=0; i<maxH; i++)
		{
			for (short e=0; e<m_Width; e++)
			{
				const short index = i*m_Width+e;
				sf::Vector2i pos = sf::Vector2i(e*TILE_SIZE + m_OffsetX, i*TILE_SIZE + m_OffsetY - m_Height * TILE_SIZE);
				sf::Vertex* quad = &m_Tilemap[index * 3 * 2];

				sf::Color calcColor = sf::Color::Transparent;
				quad[0].color = calcColor;
				quad[1].color = calcColor;
				quad[2].color = calcColor;
				quad[3].color = calcColor;
				quad[4].color = calcColor;
				quad[5].color = calcColor;

				if (pTiles[index].m_Index&CTile::SOLID)
				{
					quad[0].position = sf::Vector2f(pos.x, pos.y);
					quad[1].position = sf::Vector2f(pos.x+TILE_SIZE, pos.y+TILE_SIZE);
					quad[2].position = sf::Vector2f(pos.x+TILE_SIZE, pos.y);
					quad[3].position = sf::Vector2f(pos.x, pos.y);
					quad[4].position = sf::Vector2f(pos.x, pos.y+TILE_SIZE);
					quad[5].position = sf::Vector2f(pos.x+TILE_SIZE, pos.y+TILE_SIZE);

					int tu = pTiles[index].m_TexIndex%6;
					int tv = pTiles[index].m_TexIndex/6;

					quad[0].texCoords = sf::Vector2f(tu * tsize, tv * tsize);
					quad[1].texCoords = sf::Vector2f((tu + 1) * tsize, (tv + 1) * tsize);
					quad[2].texCoords = sf::Vector2f((tu + 1) * tsize, tv * tsize);
					quad[3].texCoords = sf::Vector2f(tu * tsize, tv * tsize);
					quad[4].texCoords = sf::Vector2f(tu * tsize, (tv + 1) * tsize);
					quad[5].texCoords = sf::Vector2f((tu + 1) * tsize, (tv + 1) * tsize);

					calcColor = sf::Color::White;
					if (m_Status == END)
						calcColor = sf::Color::Red;

					quad[0].color = calcColor;
					quad[1].color = calcColor;
					quad[2].color = calcColor;
					quad[3].color = calcColor;
					quad[4].color = calcColor;
					quad[5].color = calcColor;

					m_NumTilesRendered++;
				}
			}
		}

		m_pCore->Window()->draw(m_Tilemap, m_pCore->TextureManager()->get(CTextureManager::TEXTURE_TILESET));

		++it;
	}
}

void CMap::setZone(int zone)
{
	if (zone < 0 || zone >= NUM_ZONES)
		return;

	m_Zone = zone;
	m_OffsetY = metersToZone[m_Zone]*PIXELS_IN_METER;

	if (m_Zone == ZONE_A)
	{
		m_pCore->SoundManager()->playBackgroundMusic(CSoundManager::MUSIC_ZONE_A);
		m_pCore->setBackgroundColor(sf::Color::Black);
	}
	if (m_Zone == ZONE_B)
	{
		m_pCore->SoundManager()->playBackgroundMusic(CSoundManager::MUSIC_ZONE_B);
		m_pCore->setBackgroundColor(sf::Color(0, 11, 24));
	}
	if (m_Zone == ZONE_C)
	{
		m_pCore->SoundManager()->playBackgroundMusic(CSoundManager::MUSIC_ZONE_C);
		m_pCore->setBackgroundColor(sf::Color(0, 27, 55));
	}
	if (m_Zone == ZONE_D)
	{
		m_pCore->SoundManager()->playBackgroundMusic(CSoundManager::MUSIC_FINAL_BOSS);
		m_pCore->setBackgroundColor(sf::Color(25, 6, 6));
	}
}

void CMap::renderGrid()
{
    for (int i=0; i<SIZE_W; i++)
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(i*TILE_SIZE, 0), sf::Color::Red),
            sf::Vertex(sf::Vector2f(i*TILE_SIZE, RSIZE_H), sf::Color::Red)
        };

        m_pCore->Window()->draw(line, 2, sf::Lines);
    }
    for (int i=0; i<SIZE_H; i++)
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(0, i*TILE_SIZE + (int)m_OffsetY%TILE_SIZE), sf::Color::Red),
            sf::Vertex(sf::Vector2f(RSIZE_W, i*TILE_SIZE + (int)m_OffsetY%TILE_SIZE), sf::Color::Red)
        };

        m_pCore->Window()->draw(line, 2, sf::Lines);
    }
}

CTile* CMap::getTileAt(sf::Vector2f &pos)
{
	int x = (int)pos.x/TILE_SIZE;
	int y = (int)(pos.y-(int)m_OffsetY%TILE_SIZE)/TILE_SIZE;

	if (x >= m_Width || y >= m_Height || x < 0 || y < 0)
		return 0x0;

	int index = y*m_Width + x;
	std::list<CGroupTile>::iterator it = m_GroupTiles.begin();
	std::advance(it, m_IndexGroupTileGame);
	return &((*it).m_pTiles[index]);
}

