/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_DATA_FILE
#define H_DATA_FILE

class CConfigFile
{
	static const char FILE_SIGN[];

public:
	CConfigFile(const char *file);
	~CConfigFile();

	char m_aFilePath[512];

	int m_LowGraphics;
	int m_Vibration;
	short m_Language;
	float m_BestDistance;
	int m_BestScore;
	int m_Debug;

private:
	void load();
	void save();
};

#endif
