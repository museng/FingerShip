/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_SQLITE
#define H_SQLITE
#include "external/sqlite/sqlite3.h"

class CSqlite
{
public:
	CSqlite();
	~CSqlite();

	bool init();

private:
	sqlite3 *m_pDataBase;
};

#endif
