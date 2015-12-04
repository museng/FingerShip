/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "boss_spacestation.h"
#include "projectile.h"
#include "../engine/game_core.h"
#include "../engine/locale.h"
#include "../engine/basic_functions.h"
#include "../engine/effects.h"
#include "../engine/android_utils.h"
#include "../screens/screen_game.h"
#include <cmath>

CBossSpaceStation::CBossSpaceStation(sf::Vector2f pos)
: CEntity(CEntity::SPACESTATION, pos, 196, 196, -1),
  m_VisibleQuad(pos, 315, 310, CTextureManager::TEXTURE_SPACESTATION)
{
	m_Position = pos;
	m_Health = 100;
	m_ToUp = false;
	m_StatusTime = 75L;
	m_Rotation = 0.0f;
	m_RotDir = 1;
	m_Shoots = 0;
	m_Dead = false;
	m_ShootTime = 10;
	m_ShootTimer = 0L;
	m_DeadAnimTimer = 0L;
	m_Timer = 0L;
	m_Offset = 0.0f;

	m_Status = NORMAL_ATTACK;
}
CBossSpaceStation::~CBossSpaceStation()
{ }

void CBossSpaceStation::destroy()
{
	Core()->getScreen()->onSignal(CScreen::SIGNAL_BOSS_SPACESTATION_DETROYED);
	CEntity::destroy();
}


void CBossSpaceStation::tick()
{
	if (!m_Dead)
	{
		// Self Destroy
		if (m_Health <= 0)
		{
			Core()->getScreen()->onSignal(CScreen::SIGNAL_GAME_FINISH);
			Core()->SoundManager()->stopBackgroundMusic();
			m_Dead = true;
			m_DeadAnimTimer = 0L;
			m_Health = 0;
		}

		// Status
		if (m_Timer*0.05f > m_StatusTime)
		{
			m_Status = (m_Status == NORMAL_ATTACK)?HARD_ATTACK:NORMAL_ATTACK;
			m_StatusTime = random_int(15, 75);
			m_Timer = 0L;
			int dirs[] = { -1, 1 };
			m_RotDir = dirs[random_int(0, 1)];

			m_ShootTime = (m_Status == NORMAL_ATTACK)?10:2;
		}

		// Move
		if (!Core()->isPaused())
		{
			if (m_Status == NORMAL_ATTACK)
			{
				m_Position.y += 5.0f * (m_Position.y > TILE_SIZE)?-1:1;
				m_Rotation += 0.4f * m_RotDir;
			}
			else if (m_Status == HARD_ATTACK)
			{
				m_Position.y += 9.0f * (m_Position.y > RSIZE_H/2)?-1:1;
				m_Rotation += 1.0f * m_RotDir;
			}
		}

		// Draw Spacestation
		m_VisibleQuad.setRotation(m_Rotation);
		m_Quad.setRotation(m_Rotation);
		m_VisibleQuad.setPosition(m_Position);
		m_Quad.setPosition(m_Position);
		Core()->Window()->draw(m_VisibleQuad);

		// Draw Health
		char buff[4]={0};
		snprintf(buff, sizeof(buff), "%d", m_Health);
		sf::Text text;
		text.setFont(Core()->getDefaultFont());
		text.setString(buff);
		text.setStyle(sf::Text::Bold);
		text.setPosition(m_Position - sf::Vector2f(text.getLocalBounds().width/2, text.getLocalBounds().height/2));
		text.setColor(sf::Color::Yellow);
		Core()->Window()->draw(text);

		// Shoot
		sf::Vector2f orgSpawnPos(m_Position.x, m_Position.y+95.0f);
		sf::Vector2f vSpawns[] = { orgSpawnPos, orgSpawnPos, orgSpawnPos, orgSpawnPos, orgSpawnPos };

		vector_rotate(m_Position, &vSpawns[0], angle_to_radians(m_Rotation));
		vector_rotate(m_Position, &vSpawns[1], angle_to_radians(m_Rotation+295.0f));
		vector_rotate(m_Position, &vSpawns[2], angle_to_radians(m_Rotation+215.0f));
		vector_rotate(m_Position, &vSpawns[3], angle_to_radians(m_Rotation+145.0f));
		vector_rotate(m_Position, &vSpawns[4], angle_to_radians(m_Rotation+65.0f));

		if (m_ShootTimer > m_ShootTime)
		{
			if (m_Status == NORMAL_ATTACK || (m_Status == HARD_ATTACK && m_Shoots < 30))
			{
				for (int i=0; i<5; i++)
				{
					sf::Vector2f sdir = vector_normalize(vSpawns[i] - m_Position);
					Core()->getScreen()->addEntity(new CProjectile(getType(), vSpawns[i], sf::Color::Red, sdir, (m_Status == NORMAL_ATTACK)?15.0f:5.0f, (m_Status == NORMAL_ATTACK)?0:1));
				}
			}

			m_ShootTimer = 0L;
			m_Shoots++;
		}
		if (m_Shoots > 55)
			m_Shoots = 0;

		if (!Core()->isPaused())
		{
			m_Timer++;
			m_ShootTimer++;
		}
	}
	else
	{
		m_Position.y += 9.0f * (m_Position.y > RSIZE_H/2)?-1:1;

		int gray = random_int(45, 185);
		m_Quad.setPosition(sf::Vector2f(m_Position.x, m_Position.y));
		m_VisibleQuad.setPosition(sf::Vector2f(m_Position.x, m_Position.y));
		m_VisibleQuad.setFillColor(sf::Color(gray, gray, gray));
		Core()->Window()->draw(m_VisibleQuad);

		if (m_DeadAnimTimer >= 300L)
			destroy();

		if (m_DeadAnimTimer < 300L && m_ShootTimer > 10)
		{
			sf::FloatRect collRect = m_Quad.getGlobalBounds();

			CAndroidUtils::vibrate(sf::milliseconds(20));
			Core()->SoundManager()->play(CSoundManager::SOUND_EXPLOSION);
			CEffects::createExplosion(sf::Vector2f(random_int(collRect.left, collRect.left+collRect.width), random_int(collRect.top, collRect.top+collRect.height)), true);
			m_ShootTimer = 0L;
		}

		if (!Core()->isPaused())
		{
			m_DeadAnimTimer++;
			m_ShootTimer++;
		}
	}
}
