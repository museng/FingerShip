/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_SOUND_MANAGER
#define H_SOUND_MANAGER
#include <SFML/Audio.hpp>
#include <list>

class CSoundManager
{
public:
	enum
	{
		SOUND_SHOOT=0,
		SOUND_INTRO,
		SOUND_EXPLOSION,
		SOUND_MENU_SELECT,
		SOUND_GAME_OVER,
		SOUND_INIT,
		SOUND_DESTROY,
		SOUND_MENU_SELECT_B,
		SOUND_MENU_CHECK,
		SOUND_FIREWORK,
		SOUND_WINNER,
		SOUND_LASER_A,
		SOUND_LASER_B,
		SOUND_WRONG,
		SOUND_NO_AMMO,
		SOUND_TURRET_SHOOT,
		SOUND_SPACEMAN_DEAD,
		SOUND_SPACEMAN,
		SOUND_SHIELD_DOWN,
		SOUND_SHIELD_UP,
		SOUND_POWERUP_BAD_BOLT_A,
		SOUND_POWERUP_BAD_BOLT_B,
		SOUND_POWERUP,
		SOUND_BOSS_DAMAGE,
		MAX_SOUNDS,

		MUSIC_INTRO=0,
		MUSIC_ZONE_A,
		MUSIC_ZONE_B,
		MUSIC_ZONE_C,
		MUSIC_FINAL_BOSS,
		MUSIC_WINNER,
		MAX_SONGS,
	};

	CSoundManager();
	~CSoundManager();

	void playBackgroundMusic(int id);
	void stopBackgroundMusic();
	bool isBackgroundMusicPlaying() const { return m_BackgroundMusic.getStatus() == sf::Sound::Playing; }

	sf::Sound* play(int soundId);
	void pauseAll();
	void resumeAll();
	void stopAll();

	bool load();

	void checkPlayedSounds();
	void setSfxActive(bool status);
	void setMusicActive(bool status);
	bool isSfxActive() const { return m_Sfx; }
	bool isMusicActive() const { return m_Music; }

	int getNumPlayingSound() const { return m_vpPlaySounds.size(); }

protected:
	sf::Music m_BackgroundMusic;
	std::list<sf::Sound*> m_vpPlaySounds;
	sf::SoundBuffer m_vSounds[MAX_SOUNDS];

	bool m_Sfx;
	bool m_Music;

private:
	int m_LastMusicId;
};

#endif
