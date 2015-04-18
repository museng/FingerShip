/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
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
	bool errors = false;

    m_vpTextures[TEXTURE_HAND] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_HAND]->loadFromFile("textures/main/hand.png");
    m_vpTextures[TEXTURE_TILESET] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_TILESET]->loadFromFile("textures/main/tileset_48x48.png");
    m_vpTextures[TEXTURE_SHIP] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_SHIP]->loadFromFile("textures/main/ship.png");
    m_vpTextures[TEXTURE_SFML] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_SFML]->loadFromFile("textures/main/sfml.png");
    m_vpTextures[TEXTURE_SHIP_SHIELD] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_SHIP_SHIELD]->loadFromFile("textures/main/ship_shield.png");
    m_vpTextures[TEXTURE_PAUSE] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_PAUSE]->loadFromFile("textures/main/pause.png");
    m_vpTextures[TEXTURE_RESUME] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_RESUME]->loadFromFile("textures/main/resume.png");

    m_vpTextures[TEXTURE_PARTICLE] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_PARTICLE]->loadFromFile("textures/effects/particle.png");
    m_vpTextures[TEXTURE_FIRE] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_FIRE]->loadFromFile("textures/effects/fire.png");
    m_vpTextures[TEXTURE_MINE] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_MINE]->loadFromFile("textures/enemies/mine.png");

    m_vpTextures[TEXTURE_EXPLOSION] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_EXPLOSION]->loadFromFile("textures/effects/explosion.png");
    m_vpTextures[TEXTURE_SMOKE] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_SMOKE]->loadFromFile("textures/effects/smoke.png");
    m_vpTextures[TEXTURE_BULLET_TURRET] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_BULLET_TURRET]->loadFromFile("textures/effects/bullet_turret.png");
    m_vpTextures[TEXTURE_BULLET] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_BULLET]->loadFromFile("textures/effects/bullet.png");
    m_vpTextures[TEXTURE_BULLET_SPARK] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_BULLET_SPARK]->loadFromFile("textures/effects/bullet_spark.png");
    m_vpTextures[TEXTURE_TRASH_SPARK] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_TRASH_SPARK]->loadFromFile("textures/effects/trash_spark.png");
    m_vpTextures[TEXTURE_EXPLOSION_RING] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_EXPLOSION_RING]->loadFromFile("textures/effects/explosion_ring.png");
    m_vpTextures[TEXTURE_STAR] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_STAR]->loadFromFile("textures/effects/star.png");
    m_vpTextures[TEXTURE_BULLET_SPACESTATION] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_BULLET_SPACESTATION]->loadFromFile("textures/effects/bullet_spacestation.png");

    m_vpTextures[TEXTURE_TURRET] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_TURRET]->loadFromFile("textures/enemies/guard.png");
    m_vpTextures[TEXTURE_SHIP_ENEMY] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_SHIP_ENEMY]->loadFromFile("textures/enemies/ship_enemy.png");
    m_vpTextures[TEXTURE_BARRIER] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_BARRIER]->loadFromFile("textures/enemies/barrier.png");
    m_vpTextures[TEXTURE_SPACESTATION] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_SPACESTATION]->loadFromFile("textures/enemies/spacestation.png");
    m_vpTextures[TEXTURE_METEOR_1] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_METEOR_1]->loadFromFile("textures/enemies/meteor_1.png");
    m_vpTextures[TEXTURE_METEOR_2] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_METEOR_2]->loadFromFile("textures/enemies/meteor_2.png");
    m_vpTextures[TEXTURE_METEOR_3] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_METEOR_3]->loadFromFile("textures/enemies/meteor_3.png");
    m_vpTextures[TEXTURE_CUBE] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_CUBE]->loadFromFile("textures/enemies/cube.png");

    m_vpTextures[TEXTURE_SPACEMAN] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_SPACEMAN]->loadFromFile("textures/powerups/spaceman.png");
    m_vpTextures[TEXTURE_POWERUP_BOLT] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_POWERUP_BOLT]->loadFromFile("textures/powerups/bolt.png");
    m_vpTextures[TEXTURE_POWERUP_BAD_BOLT] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_POWERUP_BAD_BOLT]->loadFromFile("textures/powerups/bad_bolt.png");
    m_vpTextures[TEXTURE_POWERUP_SHIELD] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_POWERUP_SHIELD]->loadFromFile("textures/powerups/shield.png");
    m_vpTextures[TEXTURE_POWERUP_STAR] = new sf::Texture();
    errors = m_vpTextures[TEXTURE_POWERUP_STAR]->loadFromFile("textures/powerups/star.png");

    return errors;
}

sf::Texture* CTextureManager::get(int textId)
{
	if (textId < 0 || textId >= MAX_TEXTURES)
		return 0x0;

	return m_vpTextures[textId];
}
