/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "meteor.h"
#include "../engine/basic_functions.h"
#include "../engine/game_core.h"
#include "../engine/locale.h"
#include "../engine/effects.h"
#include <cmath>

CMeteor::CMeteor(sf::Vector2f pos)
: CEntity(CEntity::METEOR, pos, CTextureManager::TEXTURE_METEOR_1 + random_int(0, 2))
{
	m_Position = pos;


	sf::Vector2f rawDir(0.0f, 1.0f);
	if (Core()->Player()->m_pCharacter)
		rawDir = Core()->Player()->m_pCharacter->getPosition() - m_Position;
	m_Dir = sf::Vector2f(rawDir.x, rawDir.y);
	m_Dir = vector_normalize(m_Dir);

	int rand = random_int(TILE_SIZE/2, TILE_SIZE);
	sf::Vector2f size(rand, rand);
	m_Quad.setSize(size.x, size.y);

	m_Vel = random_int(1, 3);
	m_RotDir = random_int(-5, 5);
}
CMeteor::CMeteor(sf::Vector2f pos, sf::Vector2f dir, int sizel)
: CEntity(CEntity::METEOR, pos, CTextureManager::TEXTURE_METEOR_1 + random_int(0, 2))
{
	m_Dir = dir;
	m_Position = pos;

	int rand = random_int(sizel/4, sizel/2);
	if (rand <= 20)
		setToDelete();

	sf::Vector2f size(rand, rand);
	m_Quad.setSize(size.x, size.y);

	m_Vel = random_int(1, 3);
	m_RotDir = random_int(-5, 5);
}
CMeteor::~CMeteor()
{ }

void CMeteor::destroy()
{
	CEffects::createTrashSpark(m_Position);

	CEntity::destroy();
}


void CMeteor::tick()
{
	float vel = Core()->getGameSpeed();
	m_Position = sf::Vector2f(m_Position.x + m_Dir.x * vel, m_Position.y + m_Dir.y * vel);

	if (m_Position.y > RSIZE_H + m_Quad.getLocalBounds().height || m_Position.x+m_Quad.getLocalBounds().width/2 < 0 || m_Position.x-m_Quad.getLocalBounds().width/2 > RSIZE_W)
	{
		setToDelete();
		return;
	}

	m_Quad.setPosition(sf::Vector2f(m_Position.x, m_Position.y));
	m_Quad.setRotation(m_Quad.getRotation()+m_RotDir);

	Core()->Window()->draw(m_Quad);

    if (!Core()->m_AsteroidKilled)
    {
    	sf::RectangleShape rectShape(sf::Vector2f(m_Quad.getLocalBounds().width, m_Quad.getLocalBounds().height));
    	rectShape.setPosition(sf::Vector2f(m_Position.x, m_Position.y));
    	rectShape.setFillColor(sf::Color::Transparent);
    	rectShape.setOutlineThickness(2.0f);
    	rectShape.setOutlineColor(sf::Color::Red);
    	rectShape.setOrigin(sf::Vector2f(m_Quad.getLocalBounds().width/2, m_Quad.getLocalBounds().height/2));
    	Core()->Window()->draw(rectShape);

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(m_Position.x - m_Quad.getLocalBounds().width/2, m_Position.y - m_Quad.getLocalBounds().height/2 - 1.0f), sf::Color::Red),
            sf::Vertex(sf::Vector2f(m_Position.x - m_Quad.getLocalBounds().width/2 + 200.0f, m_Position.y - m_Quad.getLocalBounds().height/2 - 1.0f), sf::Color::Red)
        };
        Core()->Window()->draw(line, 2, sf::Lines);

        sf::Text text;
        text.setString(CLocale::getString(RESOURCE_STR_DISPARALE_PARA_DESTRUILA));
        text.setFont(Core()->getDefaultFont());
        text.setCharacterSize(20.0f);
        text.setOrigin(0, text.getLocalBounds().height);
        text.setPosition(sf::Vector2f(m_Position.x - m_Quad.getLocalBounds().width/2, m_Position.y - m_Quad.getLocalBounds().height/2 - 5.0f));
        text.setColor(sf::Color::Cyan);
        Core()->Window()->draw(text);
    }
}
