/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "data_file.h"
#include "locale.h"
#include "android_utils.h"
#include "basic_functions.h"
#include "game_core.h"
#include <fstream>

const char CConfigFile::FILE_SIGN[] = {'f','n','g'};
CConfigFile::CConfigFile(const char *file)
{
	CAndroidUtils::get_internal_file_path(m_aFilePath, sizeof(m_aFilePath), file);

	m_LowGraphics = 0;
	m_Vibration = 1;
	m_Language = LANGUAGE_ES;
	m_BestDistance = 0.0f;
	m_BestScore = 0;
	m_Debug = 0;

	load();
}
CConfigFile::~CConfigFile()
{
	save();
}

void CConfigFile::load()
{
	std::ifstream infile;
	infile.open(m_aFilePath, std::ios::binary | std::ios::in);
	if (!infile.is_open())
		return;

	// Check file type
	char fileSign[sizeof(FILE_SIGN)]={0};
	infile.read(fileSign, sizeof(FILE_SIGN));
	if (strncmp(fileSign, FILE_SIGN, sizeof(fileSign)) != 0)
		return;

	// Get Config
	infile.read(reinterpret_cast<char*>(&m_LowGraphics), sizeof(m_LowGraphics));
	infile.read(reinterpret_cast<char*>(&m_Vibration), sizeof(m_Vibration));
	infile.read(reinterpret_cast<char*>(&m_Language), sizeof(m_Language));
	infile.read(reinterpret_cast<char*>(&m_BestDistance), sizeof(m_BestDistance));
	infile.read(reinterpret_cast<char*>(&m_BestScore), sizeof(m_BestScore));

	infile.close();
}

void CConfigFile::save()
{
	std::ofstream outfile;
	outfile.open(m_aFilePath, std::ios::binary | std::ios::out);
	if (!outfile.is_open())
		return;

	outfile.write(FILE_SIGN, sizeof(FILE_SIGN));
	outfile.write(reinterpret_cast<const char*>(&m_LowGraphics), sizeof(m_LowGraphics));
	outfile.write(reinterpret_cast<const char*>(&m_Vibration), sizeof(m_Vibration));
	outfile.write(reinterpret_cast<const char*>(&m_Language), sizeof(m_Language));
	outfile.write(reinterpret_cast<const char*>(&m_BestDistance), sizeof(m_BestDistance));
	outfile.write(reinterpret_cast<const char*>(&m_BestScore), sizeof(m_BestScore));

	outfile.close();
}
