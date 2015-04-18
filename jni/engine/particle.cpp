/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "particle.h"
#include "basic_functions.h"
#include "game_core.h"
#include "effects.h"

CParticle::CParticle(sf::BlendMode blendMode, int render)
{
    m_Pos = m_Dir = m_SizeInit = m_SizeEnd = sf::Vector2f(0.0f, 0.0f);
    m_ColorInit = m_ColorEnd = sf::Color(255, 255, 255, 255);
    m_Vel = 0.0f;
    m_VelRot = 0.0f;
    m_Render = render;
    m_ModeText = false;
    m_DoMove = true;
    m_BlendMode = blendMode;
    m_Timer = 0L;
    m_Force = sf::Vector2f(0.0f, 0.0f);
    m_Collide = false;
}

void CParticle::tick()
{
	CGameCore *pCore = CGameCore::getInstance();

    sf::Vector2f sizeStep = sf::Vector2f(((m_SizeEnd.x-m_SizeInit.x)/m_Duration)*m_Timer*0.05f, ((m_SizeEnd.y-m_SizeInit.y)/m_Duration)*m_Timer*0.05f);
    sf::Vector3f colorStep = sf::Vector3f(((m_ColorEnd.r-m_ColorInit.r)/m_Duration)*m_Timer*0.05f, ((m_ColorEnd.g-m_ColorInit.g)/m_Duration)*m_Timer*0.05f, ((m_ColorEnd.b-m_ColorInit.b)/m_Duration)*m_Timer*0.05f);
    float colorAlphaStep = ((m_ColorEnd.a-m_ColorInit.a)/m_Duration)*m_Timer*0.05f;

    if (!pCore->isPaused())
    {
		sf::Vector2f lastPos = m_Pos;
		m_Pos += m_Dir*m_Vel-m_Force*(float)m_Timer*0.05f;

		if (m_Collide)
		{
			sf::Vector2f rPos;
			if (pCore->Collision()->intersectLineTileSolid(lastPos, m_Pos, &rPos))
			{
				if (rPos.x != m_Pos.x) m_Dir.x *= -1.0f;
				if (rPos.y != m_Pos.y) m_Dir.y *= -1.0f;

				rPos.y -= (int)pCore->Collision()->Map()->m_OffsetY%TILE_SIZE;
				m_Pos = rPos;
				m_Vel *= 0.75f;
			}
		}
    }


    if (m_DoMove)
    	m_Pos.y += pCore->getGameSpeed();

    if (!m_ModeText)
    {
    	if (m_Pos.y - m_Shape.getLocalBounds().height/2 > RSIZE_H)
    	{
    		m_Duration = 0;
    		return;
    	}

    	if (!pCore->isPaused())
    		m_Shape.setRotation(m_Shape.getRotation()+m_VelRot);
		m_Shape.setPosition(m_Pos);
		m_Shape.setSize(m_SizeInit+sizeStep);
		m_Shape.setOrigin((m_SizeInit.x+sizeStep.x)/2, (m_SizeInit.y+sizeStep.y)/2);
		m_Shape.setFillColor(sf::Color(m_ColorInit.r+colorStep.x, m_ColorInit.g+colorStep.y, m_ColorInit.b+colorStep.z, m_ColorInit.a+colorAlphaStep));

		pCore->Window()->draw(m_Shape, m_BlendMode);
    } else
    {
    	if (m_Pos.y - m_String.getLocalBounds().height/2 > RSIZE_H)
    	{
    		m_Duration = 0;
    		return;
    	}

    	m_String.setFont(pCore->getDefaultFont());
    	if (!pCore->isPaused())
    		m_String.setRotation(m_String.getRotation()+m_VelRot);
    	m_String.setPosition(m_Pos);
    	m_String.setScale((m_SizeInit.x+sizeStep.x)/m_SizeInit.x, (m_SizeInit.y+sizeStep.y)/m_SizeInit.y);
    	m_String.setOrigin(m_String.getLocalBounds().width/2, m_String.getLocalBounds().height/2);
    	m_String.setColor(sf::Color(m_ColorInit.r+colorStep.x, m_ColorInit.g+colorStep.y, m_ColorInit.b+colorStep.z, m_ColorInit.a+colorAlphaStep));

    	pCore->Window()->draw(m_String, m_BlendMode);
    }

    if (!pCore->isPaused())
    	m_Timer++;
}


// ** PARTICLE STAR ** //
CParticleStar::CParticleStar(int render)
:
	CParticle(sf::BlendAdd, render)
{
	static sf::Color colors[] = { sf::Color(25,140,255), sf::Color(247,162,46), sf::Color(206,206,0) };

	CGameCore *pCore = CGameCore::getInstance();
	float speed = pCore->getGameSpeed();

	m_Duration = -1;
	m_Length = speed - speed/2;
	m_Length = std::max(1.0f, m_Length);
	m_Length = random_int(1, m_Length);

	if (random_int(0,20) == 8) m_Color = colors[random_int(0,2)];
	else m_Color = sf::Color::White;

	m_Speed = random_float(3.5f, speed + speed/4);
}

void CParticleStar::tick()
{
	CGameCore *pCore = CGameCore::getInstance();
	if (pCore->getGameSpeed() > 0.0f)
		m_Pos.y += m_Speed;

	if (m_Pos.y > RSIZE_H)
	{
		m_Duration = 0;
		return;
	}


    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(m_Pos.x, m_Pos.y), m_Color),
        sf::Vertex(sf::Vector2f(m_Pos.x, m_Pos.y+m_Length), m_Color)
    };

    pCore->Window()->draw(line, 2, sf::Lines, m_BlendMode);
}

// ** PARTICLE FIREWORK ** //
CParticleFirework::CParticleFirework(int render)
:
	CParticle(sf::BlendAdd, render)
{
}

void CParticleFirework::tick()
{
	CGameCore *pCore = CGameCore::getInstance();
	CParticle::tick();

	sf::Vector2f sizeStep = sf::Vector2f(((m_SizeEnd.x-m_SizeInit.x)/m_Duration)*m_Timer*0.05f, ((m_SizeEnd.y-m_SizeInit.y)/m_Duration)*m_Timer*0.05f);
	sf::Vector3f colorStep = sf::Vector3f(((m_ColorEnd.r-m_ColorInit.r)/m_Duration)*m_Timer*0.05f, ((m_ColorEnd.g-m_ColorInit.g)/m_Duration)*m_Timer*0.05f, ((m_ColorEnd.b-m_ColorInit.b)/m_Duration)*m_Timer*0.05f);
	float colorAlphaStep = ((m_ColorEnd.a-m_ColorInit.a)/m_Duration)*m_Timer*0.05f;

	CParticle *pParticle = new CParticle(sf::BlendAdd, RENDER_BACK);
	pParticle->m_Pos = m_Pos;
	pParticle->m_SizeInit = m_SizeInit+sizeStep;
	pParticle->m_SizeEnd = sf::Vector2f(0.0f, 0.0f);
	pParticle->m_ColorInit = sf::Color(m_ColorInit.r+colorStep.x, m_ColorInit.g+colorStep.y, m_ColorInit.b+colorStep.z, m_ColorInit.a+colorAlphaStep);
	pParticle->m_ColorEnd = sf::Color::Transparent;
	pParticle->m_Duration = m_LightDuration;
	pParticle->m_Shape.setTexture(pCore->TextureManager()->get(CTextureManager::TEXTURE_PARTICLE));
	pParticle->m_DoMove = false;
	pCore->getScreen()->addParticle(pParticle);
}
