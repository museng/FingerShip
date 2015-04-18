/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_TEXTURE_MANAGER
#define H_TEXTURE_MANAGER
#include <SFML/Graphics.hpp>

class CTextureManager
{
public:
	enum
	{
		TEXTURE_HAND=0,
		TEXTURE_TILESET,
		TEXTURE_PARTICLE,
		TEXTURE_FIRE,
		TEXTURE_SHIP,
		TEXTURE_MINE,
		TEXTURE_EXPLOSION,
		TEXTURE_SMOKE,
		TEXTURE_TURRET,
		TEXTURE_BULLET_TURRET,
		TEXTURE_SHIP_ENEMY,
		TEXTURE_BULLET,
		TEXTURE_BULLET_SPARK,
		TEXTURE_BARRIER,
		TEXTURE_TRASH_SPARK,
		TEXTURE_EXPLOSION_RING,
		TEXTURE_SFML,
		TEXTURE_SPACEMAN,
		TEXTURE_STAR,
		TEXTURE_SPACESTATION,
		TEXTURE_SHIP_SHIELD,
		TEXTURE_METEOR_1,
		TEXTURE_METEOR_2,
		TEXTURE_METEOR_3,
		TEXTURE_CUBE,
		TEXTURE_BULLET_SPACESTATION,
		TEXTURE_PAUSE,
		TEXTURE_RESUME,

		TEXTURE_POWERUP_SHIELD,
		TEXTURE_POWERUP_BOLT,
		TEXTURE_POWERUP_BAD_BOLT,
		TEXTURE_POWERUP_STAR,

		MAX_TEXTURES,
	};

	CTextureManager();
	~CTextureManager();

	sf::Texture* get(int textId);
	bool load();

protected:
	sf::Texture* m_vpTextures[MAX_TEXTURES];
};

#endif
