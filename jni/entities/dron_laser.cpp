/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "dron_laser.h"
#include "../engine/game_core.h"
#include "../engine/locale.h"
#include "../engine/effects.h"
#include <cmath>

CDronLaser::CDronLaser(sf::Vector2f pos)
: CEntity(CEntity::DRON_LASER)
{
	m_Active = false;
	m_ShootSound = false;
	m_Shoot = false;
	m_LaserEnergy = 0.0f;
	m_pSoundLaserA = 0x0;

	m_Position = pos;
	m_OrgPos = m_Position;

	sf::Vector2f size(58, 49);
	m_CollChar = sf::RectangleShape(size);
	m_CollChar.setOrigin(sf::Vector2f(size.x/2, size.y/2));
	m_CollChar.setTexture(Core()->TextureManager()->get(CTextureManager::TEXTURE_BARRIER));
}
CDronLaser::~CDronLaser()
{
	if (m_pSoundLaserA)
		m_pSoundLaserA->stop();
}

void CDronLaser::destroy()
{
	CEffects::createExplosion(m_Position, false);
	Core()->SoundManager()->play(CSoundManager::SOUND_DESTROY);

	CEntity::destroy();
}


void CDronLaser::tick()
{
	if (!m_Active && !Core()->isPaused())
	{
		m_Position.y += Core()->getGameSpeed() + 3.0f;
		m_OrgPos.y += Core()->getGameSpeed() + 3.0f;
	}

	if (m_Position.y > RSIZE_H + TILE_SIZE)
	{
		setToDelete();
		return;
	}

	float dt = m_Timer.getElapsedTime().asSeconds();
	m_Position = sf::Vector2f(m_OrgPos.x + sinf(dt)*2.5, m_OrgPos.y + cosf(dt)*2.5);
	m_CollChar.setPosition(sf::Vector2f(m_Position.x, m_Position.y));

	// Check for shoot!
	if (!m_Active && !Core()->isPaused() && Core()->Player()->m_pCharacter && m_Position.y > TILE_SIZE*2)
	{
		sf::FloatRect rect = m_CollChar.getGlobalBounds();
		for (int i= m_Position.y; i < m_Position.y+(RSIZE_H-m_Position.y/TILE_SIZE); i++)
		{
			rect.top++;
			if (rect.intersects(Core()->Player()->m_pCharacter->getCollChar().getGlobalBounds()))
			{
				m_Active = true;
				m_LaserEnergy = 0.0f;
				m_ShootSound = false;
				m_TimerShoot.restart();
				m_pSoundLaserA = Core()->SoundManager()->play(CSoundManager::SOUND_LASER_A);
				break;
			}
		}
	}

	// Do Fire!
    if (m_Active)
    {
    	const float shootTime = 0.75f;

    	float colorAlphaStep = (255/shootTime)*m_TimerShoot.getElapsedTime().asSeconds();
    	sf::Color colorLaser = sf::Color(33, 98, 166);
    	colorLaser.a = colorAlphaStep;
        sf::Vertex lineFire[] =
        {
            sf::Vertex(sf::Vector2f(m_Position.x, m_Position.y), colorLaser),
            sf::Vertex(sf::Vector2f(m_Position.x, m_Position.y+(RSIZE_H-m_Position.y)), colorLaser)
        };
        Core()->Window()->draw(lineFire, 2, sf::Lines);

        if (m_TimerShoot.getElapsedTime().asSeconds() > shootTime)
        {
        	if (!m_ShootSound)
        		Core()->SoundManager()->play(CSoundManager::SOUND_LASER_B);
        	m_ShootSound = true;

        	if (!m_Shoot) m_LaserEnergy+=2.0f;
        	else m_LaserEnergy-=2.0f;
        	if (m_LaserEnergy > m_CollChar.getLocalBounds().width/2)
        	{
        		m_LaserEnergy = m_CollChar.getLocalBounds().width/2;
        		m_Shoot = true;
        	}
        	else if (m_LaserEnergy < 0)
        	{
        		m_LaserEnergy = 0;
        		m_Active = false;
        		m_Shoot = false;
        	}

        	sf::RectangleShape rectLaserOut(sf::Vector2f(m_LaserEnergy, m_Position.y+(RSIZE_H-m_Position.y)));
        	rectLaserOut.setFillColor(sf::Color(94,173,6));
        	rectLaserOut.setOrigin(rectLaserOut.getLocalBounds().width/2, 0);
        	rectLaserOut.setPosition(m_Position.x, m_Position.y);

        	sf::RectangleShape rectLaserIn(sf::Vector2f(m_LaserEnergy/2, m_Position.y+(RSIZE_H-m_Position.y)));
        	rectLaserIn.setFillColor(sf::Color(65,121,4));
        	rectLaserIn.setOrigin(rectLaserIn.getLocalBounds().width/2, 0);
        	rectLaserIn.setPosition(m_Position.x, m_Position.y);

    		sf::FloatRect rect = m_CollChar.getGlobalBounds();
    		if (Core()->Player()->m_pCharacter)
    		{
				for (int i= m_Position.y; i < m_Position.y+(RSIZE_H-m_Position.y/TILE_SIZE); i++)
				{
					rect.top++;
					if (rect.intersects(Core()->Player()->m_pCharacter->getCollChar().getGlobalBounds()))
					{
						rectLaserOut.setSize(sf::Vector2f(m_LaserEnergy/2, Core()->Player()->m_pCharacter->getPosition().y-m_Position.y));
						rectLaserIn.setSize(sf::Vector2f(m_LaserEnergy/2, Core()->Player()->m_pCharacter->getPosition().y-m_Position.y));

						if (!Core()->Player()->m_pCharacter->hasShield())
							Core()->Player()->destroyCharacter(true);
						else
							Core()->Player()->m_pCharacter->dropShield();
						break;
					}
				}
    		}

            Core()->Window()->draw(rectLaserOut);
            Core()->Window()->draw(rectLaserIn);
        }
    }

	Core()->Window()->draw(m_CollChar);


    if (!Core()->m_DronLaserKilled)
    {
    	sf::RectangleShape rectShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    	rectShape.setPosition(sf::Vector2f(m_Position.x, m_Position.y));
    	rectShape.setFillColor(sf::Color::Transparent);
    	rectShape.setOutlineThickness(2.0f);
    	rectShape.setOutlineColor(sf::Color::Red);
    	rectShape.setOrigin(sf::Vector2f(TILE_SIZE/2, TILE_SIZE/2));
    	Core()->Window()->draw(rectShape);

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(m_Position.x - TILE_SIZE/2, m_Position.y - TILE_SIZE/2 - 1.0f), sf::Color::Red),
            sf::Vertex(sf::Vector2f(m_Position.x - TILE_SIZE/2 + 200.0f, m_Position.y - TILE_SIZE/2 - 1.0f), sf::Color::Red)
        };
        Core()->Window()->draw(line, 2, sf::Lines);

        sf::Text text;
        text.setString(CLocale::getString(RESOURCE_STR_DISPARALE_PARA_DESTRUILA));
        text.setFont(Core()->getDefaultFont());
        text.setCharacterSize(20.0f);
        text.setOrigin(0, text.getLocalBounds().height);
        text.setPosition(sf::Vector2f(m_CollChar.getGlobalBounds().left - TILE_SIZE/2, m_CollChar.getGlobalBounds().top - TILE_SIZE/2 - 5.0f));
        text.setColor(sf::Color::Cyan);
        Core()->Window()->draw(text);
    }
}
