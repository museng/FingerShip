/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_PLAYER
#define H_PLAYER
#include "../entities/character.h"

class CPlayer
{
public:
	CPlayer();
	~CPlayer();

	void destroyCharacter(bool effects);

	int getPoints() const { return m_Points; }
	void addPoints(int points);
	void clearPoints() { m_Points = 0; }

	CCharacter *m_pCharacter;

private:
	int m_Points;
};

#endif
