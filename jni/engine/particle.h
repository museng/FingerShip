/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef PARTICLES_H
#define PARTICLES_H
#include <SFML/Graphics.hpp>


enum
{
    RENDER_BACK=0,
    RENDER_FRONT,
};

class CParticle
{
public:
    CParticle(sf::BlendMode blendMode = sf::BlendAlpha, int render = RENDER_FRONT);
    virtual ~CParticle();

    virtual void tick();

    sf::Vector2f m_Pos;
    sf::Vector2f m_Dir;
    sf::Vector2f m_SizeInit;
    sf::Vector2f m_SizeEnd;
    sf::Vector2f m_Force;
    sf::Color m_ColorInit;
    sf::Color m_ColorEnd;
    long m_Timer;
    sf::RectangleShape m_Shape;
    sf::Text m_String;
    sf::BlendMode m_BlendMode;
    float m_Vel;
    float m_VelRot;
    float m_Duration;
    int m_Render;
    bool m_ModeText;
    bool m_DoMove;
    bool m_Collide;
};


class CParticleStar : public CParticle
{
public:
	CParticleStar(int render = RENDER_BACK);
	~CParticleStar();

    virtual void tick();

private:
	float m_Length;
	float m_Speed;
	sf::Color m_Color;
};

class CParticleFirework : public CParticle
{
public:
	CParticleFirework(int render = RENDER_BACK);
	~CParticleFirework();

    virtual void tick();

    float m_LightDuration;
};

#endif // PARTICLES
