/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "engine/game_core.h"


int main(int argc, char *argv[])
{
	CGameCore *pCore = CGameCore::getInstance();
	pCore->init();

	exit(0);
}
