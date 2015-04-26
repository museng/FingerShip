/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_EFFECTS
#define H_EFFECTS

#include "game_core.h"
#include "basic_functions.h"
#include "../entities/powerup.h"
#include "../entities/cube.h"
#include "../entities/boss_spacestation.h"
#include <cstdio>

class CEffects
{
public:

	static void createExplosion(sf::Vector2f pos, bool ring)
	{
		CGameCore *pCore = CGameCore::getInstance();

		const short numParticles = pCore->m_Config.m_LowGraphics?5:12;

		createTrashSpark(pos);

		CParticle *pParticle;
		if (ring)
		{
			pParticle = new CParticle();
			pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
			pParticle->m_SizeInit = sf::Vector2f(32.0f, 32.0f);
			pParticle->m_SizeEnd = sf::Vector2f(95.0f, 95.0f);
			pParticle->m_ColorEnd.a = 0;
			pParticle->m_Duration = 1.7f;
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_EXPLOSION_RING));
			pParticle->m_Vel = 0;
			pCore->getScreen()->addParticle(pParticle);

			pParticle = new CParticle();
			pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
			pParticle->m_SizeInit = sf::Vector2f(32.0f, 32.0f);
			pParticle->m_SizeEnd = sf::Vector2f(164.0f, 164.0f);
			pParticle->m_ColorEnd.a = 0;
			pParticle->m_Duration = 1.7f;
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_EXPLOSION_RING));
			pParticle->m_Vel = 0;
			pCore->getScreen()->addParticle(pParticle);
		}

		pParticle = new CParticle(sf::BlendAdd);
		pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
		pParticle->m_SizeInit = sf::Vector2f(164.0f, 164.0f);
		pParticle->m_SizeEnd = sf::Vector2f(0.0f, 0.0f);
		pParticle->m_ColorEnd.a = 0;
		pParticle->m_Duration = 2.2f;
		pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_EXPLOSION));
		pParticle->m_Vel = 0;
		pParticle->m_VelRot = -0.75;
		pCore->getScreen()->addParticle(pParticle);

		pParticle = new CParticle(sf::BlendAdd);
		pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
		pParticle->m_SizeInit = sf::Vector2f(164.0f, 164.0f);
		pParticle->m_SizeEnd = sf::Vector2f(0.0f, 0.0f);
		pParticle->m_ColorEnd.a = 0;
		pParticle->m_Duration = 2.2f;
		pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_EXPLOSION));
		pParticle->m_Vel = 0;
		pParticle->m_VelRot = 0.75;
		pCore->getScreen()->addParticle(pParticle);
	}

	static void createExplosionSmall(sf::Vector2f pos, bool ring)
	{
		CGameCore *pCore = CGameCore::getInstance();

		const short numParticles = pCore->m_Config.m_LowGraphics?5:12;

		createTrashSpark(pos);

		CParticle *pParticle;
		if (ring)
		{
			pParticle = new CParticle();
			pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
			pParticle->m_SizeInit = sf::Vector2f(16.0f, 16.0f);
			pParticle->m_SizeEnd = sf::Vector2f(47.0f, 47.0f);
			pParticle->m_ColorEnd.a = 0;
			pParticle->m_Duration = 1.7f;
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_EXPLOSION_RING));
			pParticle->m_Vel = 0;
			pCore->getScreen()->addParticle(pParticle);

			pParticle = new CParticle();
			pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
			pParticle->m_SizeInit = sf::Vector2f(16.0f, 16.0f);
			pParticle->m_SizeEnd = sf::Vector2f(82.0f, 82.0f);
			pParticle->m_ColorEnd.a = 0;
			pParticle->m_Duration = 1.7f;
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_EXPLOSION_RING));
			pParticle->m_Vel = 0;
			pCore->getScreen()->addParticle(pParticle);
		}

		pParticle = new CParticle(sf::BlendAdd);
		pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
		pParticle->m_SizeInit = sf::Vector2f(82.0f, 82.0f);
		pParticle->m_SizeEnd = sf::Vector2f(0.0f, 0.0f);
		pParticle->m_ColorEnd.a = 0;
		pParticle->m_Duration = 2.2f;
		pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_EXPLOSION));
		pParticle->m_Vel = 0;
		pParticle->m_VelRot = -0.75;
		pCore->getScreen()->addParticle(pParticle);

		pParticle = new CParticle(sf::BlendAdd);
		pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
		pParticle->m_SizeInit = sf::Vector2f(82.0f, 82.0f);
		pParticle->m_SizeEnd = sf::Vector2f(0.0f, 0.0f);
		pParticle->m_ColorEnd.a = 0;
		pParticle->m_Duration = 2.2f;
		pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_EXPLOSION));
		pParticle->m_Vel = 0;
		pParticle->m_VelRot = 0.75;
		pCore->getScreen()->addParticle(pParticle);
	}

	static void createTrashSpark(sf::Vector2f pos, sf::Color color = sf::Color::White)
	{
		CGameCore *pCore = CGameCore::getInstance();

		const short numParticles = pCore->m_Config.m_LowGraphics?5:12;
		for (int i=0; i<numParticles; i++)
		{
			CParticle *pParticle = new CParticle();
			pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
			pParticle->m_SizeInit = sf::Vector2f(32.0f, 32.0f);
			pParticle->m_SizeEnd = sf::Vector2f(32.0f, 32.0f);
			pParticle->m_ColorInit = sf::Color::Yellow;
			pParticle->m_ColorEnd = color;
			pParticle->m_ColorEnd.a = 0;
			pParticle->m_Duration = 1.5f;
			pParticle->m_Collide = true;
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_TRASH_SPARK));
			pParticle->m_Dir = sf::Vector2f(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f));
			pParticle->m_Vel = random_float(2.0f, 5.5f);
			pCore->getScreen()->addParticle(pParticle);
		}
	}

	static void createStarExplosion(sf::Vector2f pos)
	{
		CGameCore *pCore = CGameCore::getInstance();

		const short numParticles = pCore->m_Config.m_LowGraphics?8:16;
		for (int i=0; i<numParticles; i++)
		{
			CParticle *pParticle = new CParticle();
			pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
			pParticle->m_SizeInit = sf::Vector2f(32.0f, 32.0f);
			pParticle->m_SizeEnd = sf::Vector2f(24.0f, 24.0f);
			pParticle->m_ColorInit = sf::Color::White;
			pParticle->m_ColorEnd = sf::Color::White;
			pParticle->m_ColorEnd.a = 0;
			pParticle->m_Duration = 2.2f;
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_STAR));
			pParticle->m_Dir = sf::Vector2f(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f));
			pParticle->m_Vel = random_float(2.0f, 5.5f);
			pCore->getScreen()->addParticle(pParticle);
		}
	}

	static void createBloodSplash(sf::Vector2f pos)
	{
		CGameCore *pCore = CGameCore::getInstance();

		const short numParticles = pCore->m_Config.m_LowGraphics?8:16;
		for (int i=0; i<numParticles; i++)
		{
			CParticle *pParticle = new CParticle();
			pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
			pParticle->m_SizeInit = sf::Vector2f(32.0f, 32.0f);
			pParticle->m_SizeEnd = sf::Vector2f(42.0f, 42.0f);
			pParticle->m_ColorInit = sf::Color::Red;
			pParticle->m_ColorEnd = sf::Color::Red;
			pParticle->m_ColorEnd.a = 0;
			pParticle->m_Duration = 5.2f;
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_TRASH_SPARK));
			pParticle->m_Dir = sf::Vector2f(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f));
			pParticle->m_Vel = random_float(0.5f, 1.5f);
			pCore->getScreen()->addParticle(pParticle);
		}
	}

	static void createFirework(sf::Vector2f pos)
	{
		CGameCore *pCore = CGameCore::getInstance();

		const short numParticles = pCore->m_Config.m_LowGraphics?5:12;
		const sf::Color color(random_int(0, 255), random_int(0, 255), random_int(0, 255), 255);

		for (int i=0; i<numParticles; i++)
		{
			CParticleFirework *pParticle = new CParticleFirework();
			pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
			pParticle->m_SizeInit = sf::Vector2f(32.0f, 32.0f);
			pParticle->m_SizeEnd = sf::Vector2f(0.0f, 0.0f);
			pParticle->m_ColorInit = color;
			pParticle->m_ColorEnd = sf::Color(255, 244, 204);
			pParticle->m_ColorEnd.a = 0;
			pParticle->m_Duration = random_float(0.25f, 1.25f);
			pParticle->m_LightDuration = random_float(1.6f, 4.5f);
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_PARTICLE));
			pParticle->m_Dir = sf::Vector2f(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f));
			pParticle->m_Vel = random_float(5.0f, 12.5f);
			pParticle->m_Force = sf::Vector2f(0.0f, -9.8f);
			pParticle->m_DoMove = false;
			pCore->getScreen()->addParticle(pParticle);
		}
	}

	static void createBulletSpark(sf::Vector2f pos)
	{
		CGameCore *pCore = CGameCore::getInstance();

		for (int i=0; i<2; i++)
		{
			CParticle *pParticle = new CParticle();
			pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
			pParticle->m_SizeInit = sf::Vector2f(32.0f, 32.0f);
			pParticle->m_SizeEnd = sf::Vector2f(0.0f, 0.0f);
			pParticle->m_ColorInit = sf::Color::White;
			pParticle->m_ColorEnd = sf::Color::White;
			pParticle->m_ColorEnd.a = 0;
			pParticle->m_Duration = 1.55f;
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_BULLET_SPARK));
			pParticle->m_Dir = sf::Vector2f(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f));
			pParticle->m_Vel = random_float(1.5f, 3.5f);
			pCore->getScreen()->addParticle(pParticle);
		}
	}

	static void createFireTrail(sf::Vector2f pos)
	{
		CGameCore *pCore = CGameCore::getInstance();

		if (pCore->m_Config.m_LowGraphics)
			return;

		CParticle *pParticle = new CParticle(sf::BlendAdd, RENDER_BACK);
		pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
		pParticle->m_SizeInit = sf::Vector2f(CCharacter::SIZE, CCharacter::SIZE);
		pParticle->m_SizeEnd = sf::Vector2f(CCharacter::SIZE/2, CCharacter::SIZE/2);
		pParticle->m_ColorInit = sf::Color::White;
		pParticle->m_ColorEnd = sf::Color::Black;
		pParticle->m_ColorEnd.a = 0;
		pParticle->m_Duration = 1.3f;
		pParticle->m_Dir = sf::Vector2f(0.0f, 1.0f);
		pParticle->m_Vel = 6.0f;
		pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_FIRE));
		pParticle->m_DoMove = false;
		pCore->getScreen()->addParticle(pParticle);
	}

	static void createLightRedTrail(sf::Vector2f pos)
	{
		CGameCore *pCore = CGameCore::getInstance();

		if (pCore->m_Config.m_LowGraphics)
			return;

		CParticle *pParticle = new CParticle(sf::BlendAdd, RENDER_BACK);
		pParticle->m_Pos = sf::Vector2f(pos.x-28.0f, pos.y+CCharacter::SIZE/2);
		pParticle->m_SizeInit = sf::Vector2f(CCharacter::SIZE/4, CCharacter::SIZE/4);
		pParticle->m_SizeEnd = sf::Vector2f(CCharacter::SIZE/8, CCharacter::SIZE/8);
		pParticle->m_ColorInit = sf::Color::Red;
		pParticle->m_ColorEnd = sf::Color(232, 112, 42, 0);
		pParticle->m_Duration = 0.6f;
		pParticle->m_DoMove = false;
		pParticle->m_Dir = sf::Vector2f(0.0f, 1.0f);
		pParticle->m_Vel = 6.0f;
		pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_PARTICLE));
		pCore->getScreen()->addParticle(pParticle);

		pParticle = new CParticle(sf::BlendAdd, RENDER_BACK);
		pParticle->m_Pos = sf::Vector2f(pos.x+28.0f, pos.y+CCharacter::SIZE/2);
		pParticle->m_SizeInit = sf::Vector2f(CCharacter::SIZE/4, CCharacter::SIZE/4);
		pParticle->m_SizeEnd = sf::Vector2f(CCharacter::SIZE/8, CCharacter::SIZE/8);
		pParticle->m_ColorInit = sf::Color::Red;
		pParticle->m_ColorEnd = sf::Color(232, 112, 42, 0);
		pParticle->m_Duration = 0.6f;
		pParticle->m_DoMove = false;
		pParticle->m_Dir = sf::Vector2f(0.0f, 1.0f);
		pParticle->m_Vel = 6.0f;
		pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_PARTICLE));
		pCore->getScreen()->addParticle(pParticle);
	}

	static void createTurretProjectileTrail(sf::Vector2f pos)
	{
		CGameCore *pCore = CGameCore::getInstance();

		if (pCore->m_Config.m_LowGraphics)
			return;

		CParticle *pParticle = new CParticle(sf::BlendAdd, RENDER_FRONT);
		pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
		pParticle->m_SizeInit = sf::Vector2f(9.0f, 9.0f);
		pParticle->m_SizeEnd = sf::Vector2f(0.0f, 0.0f);
		pParticle->m_ColorInit = sf::Color::White;
		pParticle->m_ColorEnd = sf::Color::Black;
		pParticle->m_ColorEnd.a = 0;
		pParticle->m_Duration = 0.3f;
		pParticle->m_Dir = sf::Vector2f(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f));
		pParticle->m_Vel = random_float(1.0f, 4.5f);
		pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_BULLET_TURRET));
		pParticle->m_DoMove = false;
		pCore->getScreen()->addParticle(pParticle);
	}

	static void createCharacterProjectileTrail(sf::Vector2f pos)
	{
		CGameCore *pCore = CGameCore::getInstance();

		if (pCore->m_Config.m_LowGraphics)
			return;

		CParticle *pParticle = new CParticle(sf::BlendAdd, RENDER_BACK);
		pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
		pParticle->m_SizeInit = sf::Vector2f(22.0f, 22.0f);
		pParticle->m_SizeEnd = sf::Vector2f(0.0f, 0.0f);
		pParticle->m_ColorInit = sf::Color::White;
		pParticle->m_ColorEnd = sf::Color::Black;
		pParticle->m_ColorEnd.a = 0;
		pParticle->m_Duration = 1.0f;
		pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_BULLET));
		pParticle->m_DoMove = false;
		pCore->getScreen()->addParticle(pParticle);
	}

	static void createSmokeTrail(sf::Vector2f pos)
	{
		CGameCore *pCore = CGameCore::getInstance();

		if (pCore->m_Config.m_LowGraphics)
			return;

		CParticle *pParticle = new CParticle(sf::BlendAdd, RENDER_BACK);
		pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
		pParticle->m_SizeInit = sf::Vector2f(32.0f, 32.0f);
		pParticle->m_SizeEnd = sf::Vector2f(0.0f, 0.0f);
		pParticle->m_ColorInit = sf::Color::Black;
		pParticle->m_ColorEnd = sf::Color::White;
		pParticle->m_ColorEnd.a = 0;
		pParticle->m_Duration = 1.05f;
		pParticle->m_VelRot = random_float(-1.0f, 1.0f);
		pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_SMOKE));
		pCore->getScreen()->addParticle(pParticle);
	}

	static void createPowerupPickup(sf::Vector2f pos, int type)
	{
		CGameCore *pCore = CGameCore::getInstance();

		CParticle *pParticle = new CParticle(sf::BlendAlpha, RENDER_FRONT);
		pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
		pParticle->m_SizeInit = sf::Vector2f(TILE_SIZE, TILE_SIZE);
		pParticle->m_SizeEnd = sf::Vector2f(TILE_SIZE*2.0f, TILE_SIZE*2.0f);
		pParticle->m_ColorInit = sf::Color::White;
		pParticle->m_ColorEnd = sf::Color::Black;
		pParticle->m_ColorEnd.a = 0;
		pParticle->m_Duration = 1.45f;

		if (type == CPowerup::SHIELD)
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_POWERUP_SHIELD));
		else if (type == CPowerup::BOLT)
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_POWERUP_BOLT));
		else if (type == CPowerup::STAR)
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_POWERUP_STAR));
		else if (type == CPowerup::BAD_BOLT)
			pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_POWERUP_BAD_BOLT));

		pCore->getScreen()->addParticle(pParticle);
	}

	static void createPoints(sf::Vector2f pos, int points)
	{
		CGameCore *pCore = CGameCore::getInstance();

		CParticle *pParticle = new CParticle(sf::BlendAlpha, RENDER_FRONT);
		pParticle->m_ModeText = true;
		char buff[5];
		if (points >= 0) snprintf(buff, sizeof(buff), "+%d", points);
		else snprintf(buff, sizeof(buff), "%d", points);
		pParticle->m_String.setString(buff);
		pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
		pParticle->m_SizeInit = sf::Vector2f(18.0f, 18.0f);
		pParticle->m_SizeEnd = sf::Vector2f(32.0f, 32.0f);
		pParticle->m_ColorInit = sf::Color::Yellow;
		pParticle->m_ColorEnd = (points>=0)?sf::Color::Green:sf::Color::Red;
		pParticle->m_ColorEnd.a = 85;
		pParticle->m_Duration = 2.15f;
		pParticle->m_VelRot = random_int(0, 2) == 1?-0.45f:0.45f;
		pCore->getScreen()->addParticle(pParticle);
	}

	static void createTextFadeOutDown(sf::Vector2f pos, const char *text, sf::Color color)
	{
		CGameCore *pCore = CGameCore::getInstance();

		CParticle *pParticle = new CParticle(sf::BlendAlpha, RENDER_FRONT);
		pParticle->m_ModeText = true;
		pParticle->m_String.setString(text);
		pParticle->m_Pos = sf::Vector2f(pos.x, pos.y);
		pParticle->m_SizeInit = sf::Vector2f(18.0f, 18.0f);
		pParticle->m_SizeEnd = sf::Vector2f(24.0f, 24.0f);
		pParticle->m_ColorInit = color;
		pParticle->m_ColorEnd = sf::Color::Black;
		pParticle->m_ColorEnd.a = 0;
		pParticle->m_Duration = 2.15f;
		pParticle->m_Dir = sf::Vector2f(0, 1);
		pParticle->m_Vel = 1.25f;
		pParticle->m_DoMove = false;
		pCore->getScreen()->addParticle(pParticle);
	}

	static void createDamageCube(CCube *pCube)
	{
		CGameCore *pCore = CGameCore::getInstance();

		int rsize = pCube->getCollChar().getSize().x;
		CParticle *pParticle = new CParticle(sf::BlendAdd, RENDER_FRONT);
		pParticle->m_Pos = pCube->getPosition();
		pParticle->m_SizeInit = sf::Vector2f(rsize, rsize);
		pParticle->m_SizeEnd = sf::Vector2f(rsize, rsize);
		pParticle->m_ColorInit = sf::Color(250, 0, 0, 200);
		pParticle->m_ColorEnd = sf::Color(250, 0, 0, 0);
		pParticle->m_Duration = 0.95f;
		pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_CUBE));
		pParticle->m_Shape.setRotation(pCube->getCollChar().getRotation());
		pCore->getScreen()->addParticle(pParticle);
	}

	static void createDamageBossSpaceStation(CBossSpaceStation *pBoss)
	{
		CGameCore *pCore = CGameCore::getInstance();

		int rsize = pBoss->getCollChar().getSize().x;
		CParticle *pParticle = new CParticle(sf::BlendAdd, RENDER_FRONT);
		pParticle->m_Pos = pBoss->getPosition();
		pParticle->m_SizeInit = sf::Vector2f(rsize, rsize);
		pParticle->m_SizeEnd = sf::Vector2f(rsize, rsize);
		pParticle->m_ColorInit = sf::Color(250, 0, 0, 200);
		pParticle->m_ColorEnd = sf::Color(250, 0, 0, 0);
		pParticle->m_Duration = 0.9f;
		pParticle->m_DoMove = false;
		pParticle->m_Dir = sf::Vector2f(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f));
		pParticle->m_Vel = 3.0f;
		pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_SPACESTATION));
		pParticle->m_Shape.setRotation(pBoss->getCollChar().getRotation());
		pCore->getScreen()->addParticle(pParticle);
	}

};

#endif
