/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_SCREEN
#define H_SCREEN

#include "../engine/game_core.h"
#include "../entities/entity.h"
#include <list>

class CScreen
{
public:
	enum
	{
		INIT = 0,
		OPTIONS,
		GAME,
		GAME_FINISH,
		CREDITS,
		MAX_SCREENS,

		SIGNAL_BOSS_SPACESTATION_DETROYED=0,
		SIGNAL_GAME_FINISH,
		SIGNAL_GAINED_FOCUS,
		SIGNAL_LOST_FOCUS,
		MAX_SIGNALS,
	};

	CScreen(int camW, int camH);
	virtual ~CScreen();

	class CGameCore* Core() { return m_pCore; }
	sf::View* Camera() { return &m_Camera; }

	virtual void tick() = 0;
	virtual sf::View& updateCamera() = 0;
	virtual void onSignal(int signal) { };

	void setSize(int sW, int sH);

	// Entidades/Particulas
	std::list<class CEntity*>* getEntities() { return &m_vpEntities; }
	std::list<class CEntity*> getEntitiesByType(int type);
	void addEntity(class CEntity *pEnt);
	void delEntity(class CEntity *pEnt);
	bool addParticle(CParticle *pParticle);
	void clearEntities();
	void clearParticles();

	bool m_InputActive;

protected:
	class CGameCore *m_pCore;
	std::list<class CEntity*> m_vpEntities;
	std::list<CParticle*> m_vpParticles;

	void renderBack();
	void renderFront();

private:
	sf::View m_Camera;
};

#endif
