/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "screen.h"
#include "../engine/basic_functions.h"

CScreen::CScreen(int camW, int camH)
{
	m_pCore = CGameCore::getInstance();

	m_Camera = m_pCore->Window()->getDefaultView();
	m_Camera.setSize(camW, camH);
	m_Camera.setCenter(camW/2, camH/2);
	m_pCore->Window()->setView(m_Camera);

	m_InputActive = true;
}
CScreen::~CScreen()
{
	std::list<CEntity*>::iterator it = m_vpEntities.begin();
	while (it != m_vpEntities.end())
	{
		delete (*it);
		++it;
	}
	m_vpEntities.clear();

	std::list<CParticle*>::iterator itp = m_vpParticles.begin();
	while (itp != m_vpParticles.end())
	{
		delete (*itp);
		++itp;
	}
	m_vpParticles.clear();
}

void CScreen::setSize(int sW, int sH)
{
	m_Camera.setSize(sW, sH);
	m_Camera.setCenter(sW/2, sH/2);
	m_pCore->Window()->setView(m_Camera);
}

void CScreen::renderBack()
{
	//Particle Manage & Draw (BACK)
	std::list<CParticle*>::iterator itp = m_vpParticles.begin();
	while (itp != m_vpParticles.end())
	{
		if ((*itp)->m_Duration != -1.0f && (*itp)->m_Timer*0.05f >= (*itp)->m_Duration)
		{
			delete (*itp);
			itp = m_vpParticles.erase(itp);
		} else
		{
			if ((*itp)->m_Render == RENDER_BACK)
				(*itp)->tick();
			itp++;
		}
	}
}

void CScreen::renderFront()
{
	// Entities Manage
	std::list<CEntity*>::iterator it = m_vpEntities.begin();
	while (it != m_vpEntities.end())
	{
		if ((*it)->toDelete())
		{
			delete (*it);
			it = m_vpEntities.erase(it);
		} else
		{
			(*it)->tick();
			++it;
		}
	}

	// Particles Draw
	std::list<CParticle*>::iterator itp = m_vpParticles.begin();
	while (itp != m_vpParticles.end())
	{
		if ((*itp)->m_Render == RENDER_FRONT)
			(*itp)->tick();

		itp++;
	}
}

void CScreen::addEntity(CEntity *pEnt)
{
	m_vpEntities.push_back(pEnt);
}

void CScreen::delEntity(CEntity *pEnt)
{
	m_vpEntities.remove(pEnt);
}

std::list<class CEntity*> CScreen::getEntitiesByType(int type)
{
	std::list<class CEntity*> retList;

	std::list<CEntity*>::iterator it = m_vpEntities.begin();
	while (it != m_vpEntities.end())
	{
		if ((*it)->getType() == type)
			retList.push_back((*it));
		it++;
	}

	return retList;
}

bool CScreen::addParticle(CParticle *pParticle)
{
	if (!pParticle)
		return false;

	m_vpParticles.push_back(pParticle);
	return true;
}

void CScreen::clearEntities()
{
	std::list<CEntity*>::iterator it = m_vpEntities.begin();
	while (it != m_vpEntities.end())
	{
		(*it)->setToDelete();
		++it;
	}
}

void CScreen::clearParticles()
{
	std::list<CParticle*>::iterator itp = m_vpParticles.begin();
	while (itp != m_vpParticles.end())
	{
		(*itp)->m_Duration = 0;
		++itp;
	}
}

void CScreen::addInitStars()
{
	for (int i=0; i<RSIZE_W; i++)
	{
		for (int e=0; e<RSIZE_H; e+=TILE_SIZE/2)
		{
			if (random_int(0, 200) == 5)
			{
				CParticleStar *pStar = new CParticleStar();
				pStar->m_Pos = sf::Vector2f(i, e);
				addParticle(pStar);
			}
		}
	}
}
