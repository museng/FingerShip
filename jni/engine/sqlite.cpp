/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "sqlite.h"

CSqlite::CSqlite()
{
	m_pDataBase = nullptr;
}

CSqlite::~CSqlite()
{
	if (m_pDataBase)
	{
		sqlite3_close(m_pDataBase);
	}
}

bool CSqlite::init()
{
	int rc = sqlite3_open("test.db", &m_pDataBase);
	if (!rc)
		return false;



	return true;
}
