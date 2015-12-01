/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "player.h"
#include "effects.h"
#include "game_core.h"
#include "../screens/screen_game.h"

CPlayer::CPlayer()
{
	m_Points = 0;
	m_pCharacter = nullptr;
}
CPlayer::~CPlayer()
{ }

void CPlayer::destroyCharacter(bool effects)
{
	if (m_pCharacter != nullptr)
	{
		if (effects)
			m_pCharacter->destroy();
		else
			m_pCharacter->setToDelete();
	}

	m_pCharacter = nullptr;
}

void CPlayer::addPoints(int points)
{
	CGameCore *pCore = CGameCore::getInstance();
	CScreenGame *pScreenGame = dynamic_cast<CScreenGame*>(pCore->getScreen());
	if (pScreenGame)
		pScreenGame->createPointsEffect();

	m_Points += points;
}
