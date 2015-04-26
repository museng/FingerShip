/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "sound_manager.h"
#include "basic_functions.h"

CSoundManager::CSoundManager()
{
	m_Sfx = true;
	m_Music = true;
	m_LastMusicId = -1;
}
CSoundManager::~CSoundManager()
{
	//Stop Sounds
	stopAll();
	stopBackgroundMusic();
	checkPlayedSounds();
	m_vpPlaySounds.clear();
}

bool CSoundManager::load()
{
	return (m_vSounds[SOUND_SHOOT].loadFromFile("sfx/shoot.ogg") &&
			m_vSounds[SOUND_INTRO].loadFromFile("sfx/intro.ogg") &&
			m_vSounds[SOUND_EXPLOSION].loadFromFile("sfx/explosion.ogg") &&
			m_vSounds[SOUND_MENU_SELECT].loadFromFile("sfx/menu_select.ogg") &&
			m_vSounds[SOUND_GAME_OVER].loadFromFile("sfx/game_over.ogg") &&
			m_vSounds[SOUND_INIT].loadFromFile("sfx/init.ogg") &&
			m_vSounds[SOUND_DESTROY].loadFromFile("sfx/destroy.ogg") &&
			m_vSounds[SOUND_MENU_SELECT_B].loadFromFile("sfx/menu_select_b.ogg") &&
			m_vSounds[SOUND_MENU_CHECK].loadFromFile("sfx/menu_check.ogg") &&
			m_vSounds[SOUND_FIREWORK].loadFromFile("sfx/firework.ogg") &&
			m_vSounds[SOUND_WINNER].loadFromFile("sfx/winner.ogg") &&
			m_vSounds[SOUND_LASER_A].loadFromFile("sfx/laserA.ogg") &&
			m_vSounds[SOUND_LASER_B].loadFromFile("sfx/laserB.ogg") &&
			m_vSounds[SOUND_WRONG].loadFromFile("sfx/wrong.ogg") &&
			m_vSounds[SOUND_NO_AMMO].loadFromFile("sfx/no_ammo.ogg") &&
			m_vSounds[SOUND_TURRET_SHOOT].loadFromFile("sfx/turret_shoot.ogg") &&
			m_vSounds[SOUND_SPACEMAN_DEAD].loadFromFile("sfx/spaceman_dead.ogg") &&
			m_vSounds[SOUND_SPACEMAN].loadFromFile("sfx/spaceman.ogg") &&
			m_vSounds[SOUND_SHIELD_DOWN].loadFromFile("sfx/shield_down.ogg") &&
			m_vSounds[SOUND_SHIELD_UP].loadFromFile("sfx/shield_up.ogg") &&
			m_vSounds[SOUND_POWERUP_BAD_BOLT_A].loadFromFile("sfx/bad_bolt_a.ogg") &&
			m_vSounds[SOUND_POWERUP_BAD_BOLT_B].loadFromFile("sfx/bad_bolt_b.ogg") &&
			m_vSounds[SOUND_POWERUP].loadFromFile("sfx/powerup.ogg") &&
			m_vSounds[SOUND_BOSS_DAMAGE].loadFromFile("sfx/boss_damage.ogg"));
}

void CSoundManager::setSfxActive(bool status)
{
	m_Sfx = status;

	if (!m_Sfx)
		stopAll();
}

void CSoundManager::setMusicActive(bool status)
{
	m_Music = status;

	if (!m_Music)
		stopBackgroundMusic();
}

void CSoundManager::checkPlayedSounds()
{
	// Sound Manage
	std::list<sf::Sound*>::iterator itps = m_vpPlaySounds.begin();
	while (itps != m_vpPlaySounds.end())
	{
		if ((*itps)->getStatus() == sf::Sound::Stopped)
		{
			delete (*itps);
			(*itps) = 0x0;
			itps = m_vpPlaySounds.erase(itps);
		} else
			++itps;
	}
}

sf::Sound* CSoundManager::play(int soundId)
{
	if (!m_Sfx || soundId < 0 || soundId >= MAX_SOUNDS)
		return 0x0;

	sf::Sound *pSound = new sf::Sound();
	pSound->setBuffer(m_vSounds[soundId]);
	pSound->play();
	m_vpPlaySounds.push_back(pSound);

	return pSound;
}

void CSoundManager::pauseAll()
{
	if (m_BackgroundMusic.getStatus() != sf::Sound::Stopped)
		m_BackgroundMusic.pause();

	std::list<sf::Sound*>::iterator itps = m_vpPlaySounds.begin();
	while (itps != m_vpPlaySounds.end())
	{
		if ((*itps)->getStatus() != sf::Sound::Stopped)
			(*itps)->pause();

		++itps;
	}
}
void CSoundManager::resumeAll()
{
	if (m_BackgroundMusic.getStatus() == sf::Sound::Paused)
		m_BackgroundMusic.play();

	std::list<sf::Sound*>::iterator itps = m_vpPlaySounds.begin();
	while (itps != m_vpPlaySounds.end())
	{
		if ((*itps)->getStatus() == sf::Sound::Paused)
			(*itps)->play();

		++itps;
	}
}
void CSoundManager::stopAll()
{
	std::list<sf::Sound*>::iterator itps = m_vpPlaySounds.begin();
	while (itps != m_vpPlaySounds.end())
	{
		if ((*itps)->getStatus() != sf::Sound::Stopped)
			(*itps)->stop();

		++itps;
	}
}

void CSoundManager::playBackgroundMusic(int id)
{
	static const char *musicFiles[] = { "sfx/music_intro.ogg", "sfx/music_zone_a.ogg", "sfx/music_zone_b.ogg", "sfx/music_zone_c.ogg", "sfx/music_final_boss.ogg", "sfx/music_winner.ogg" };
	if (!m_Music || id < 0 || id >= MAX_SONGS)
		return;

	if (id != m_LastMusicId)
	{
		if (m_BackgroundMusic.getStatus() != sf::Sound::Stopped)
			m_BackgroundMusic.stop();

		m_BackgroundMusic.openFromFile(musicFiles[id]);
		m_BackgroundMusic.setLoop(true);
		m_BackgroundMusic.play();

		m_LastMusicId = id;
	}
	else if (m_BackgroundMusic.getStatus() == sf::Sound::Stopped)
		m_BackgroundMusic.play();
}

void CSoundManager::stopBackgroundMusic()
{
	if (m_BackgroundMusic.getStatus() != sf::Sound::Stopped)
		m_BackgroundMusic.stop();
}
