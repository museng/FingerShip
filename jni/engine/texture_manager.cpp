/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "texture_manager.h"

CTextureManager::CTextureManager()
{

}
CTextureManager::~CTextureManager()
{
	for (int i=0; i<MAX_TEXTURES; delete m_vpTextures[i++]);
}

bool CTextureManager::load()
{
	LOAD_TEXTURE(TEXTURE_HAND, "textures/main/hand.png")
	LOAD_TEXTURE(TEXTURE_TILESET, "textures/main/tileset_48x48.png")
	LOAD_TEXTURE(TEXTURE_SHIP, "textures/main/ship.png")
	LOAD_TEXTURE(TEXTURE_SFML, "textures/main/sfml.png")
	LOAD_TEXTURE(TEXTURE_SHIP_SHIELD, "textures/main/ship_shield.png")
	LOAD_TEXTURE(TEXTURE_PAUSE, "textures/main/pause.png")
	LOAD_TEXTURE(TEXTURE_RESUME, "textures/main/resume.png")

	LOAD_TEXTURE(TEXTURE_PARTICLE, "textures/effects/particle.png")
	LOAD_TEXTURE(TEXTURE_FIRE, "textures/effects/fire.png")
	LOAD_TEXTURE(TEXTURE_EXPLOSION, "textures/effects/explosion.png")
	LOAD_TEXTURE(TEXTURE_SMOKE, "textures/effects/smoke.png")
	LOAD_TEXTURE(TEXTURE_BULLET_TURRET, "textures/effects/bullet_turret.png")
	LOAD_TEXTURE(TEXTURE_BULLET, "textures/effects/bullet.png")
	LOAD_TEXTURE(TEXTURE_BULLET_SPARK, "textures/effects/bullet_spark.png")
	LOAD_TEXTURE(TEXTURE_TRASH_SPARK, "textures/effects/trash_spark.png")
	LOAD_TEXTURE(TEXTURE_EXPLOSION_RING, "textures/effects/explosion_ring.png")
	LOAD_TEXTURE(TEXTURE_STAR, "textures/effects/star.png")
	LOAD_TEXTURE(TEXTURE_BULLET_SPACESTATION, "textures/effects/bullet_spacestation.png")

	LOAD_TEXTURE(TEXTURE_MINE, "textures/enemies/mine.png")
	LOAD_TEXTURE(TEXTURE_TURRET, "textures/enemies/guard.png")
	LOAD_TEXTURE(TEXTURE_SHIP_ENEMY, "textures/enemies/ship_enemy.png")
	LOAD_TEXTURE(TEXTURE_BARRIER, "textures/enemies/barrier.png")
	LOAD_TEXTURE(TEXTURE_SPACESTATION, "textures/enemies/spacestation.png")
	LOAD_TEXTURE(TEXTURE_METEOR_1, "textures/enemies/meteor_1.png")
	LOAD_TEXTURE(TEXTURE_METEOR_2, "textures/enemies/meteor_2.png")
	LOAD_TEXTURE(TEXTURE_METEOR_3, "textures/enemies/meteor_3.png")
	LOAD_TEXTURE(TEXTURE_CUBE, "textures/enemies/cube.png")

	LOAD_TEXTURE(TEXTURE_SPACEMAN, "textures/powerups/spaceman.png")
	LOAD_TEXTURE(TEXTURE_POWERUP_BOLT, "textures/powerups/bolt.png")
	LOAD_TEXTURE(TEXTURE_POWERUP_BAD_BOLT, "textures/powerups/bad_bolt.png")
	LOAD_TEXTURE(TEXTURE_POWERUP_SHIELD, "textures/powerups/shield.png")
	LOAD_TEXTURE(TEXTURE_POWERUP_STAR, "textures/powerups/star.png")

    return true;
}

sf::Texture* CTextureManager::get(int textId)
{
	if (textId < 0 || textId >= MAX_TEXTURES)
		return 0x0;

	return m_vpTextures[textId];
}
