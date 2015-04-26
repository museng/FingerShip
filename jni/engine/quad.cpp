/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "quad.h"
#include "game_core.h"
#include "basic_functions.h"
#include <SFML/Graphics/Vertex.hpp>

Quad::Quad(sf::Vector2f pos, float width, float height, int textId)
: Drawable(),
  m_pvPoints(sf::Triangles, 6)
{
	m_OutLine = false;
	m_OutLineColor = sf::Color::Red;
	m_TextureId = textId;
	m_Position = pos;
	m_RotationDegrees = 0.0f;

	CGameCore *pCore = CGameCore::getInstance();
	sf::Texture *pTexture = pCore->TextureManager()->get(m_TextureId);
	if (pTexture)
	{
		m_pvPoints[0].texCoords = sf::Vector2f(0.0f, 0.0f);
		m_pvPoints[1].texCoords = sf::Vector2f(pTexture->getSize().x, pTexture->getSize().y);
		m_pvPoints[2].texCoords = sf::Vector2f(pTexture->getSize().x, 0.0f);
		m_pvPoints[3].texCoords = sf::Vector2f(0.0f, 0.0f);
		m_pvPoints[4].texCoords = sf::Vector2f(0.0f, pTexture->getSize().y);
		m_pvPoints[5].texCoords = sf::Vector2f(pTexture->getSize().x, pTexture->getSize().y);
	}

	setSize(width, height);

	for (int i=0; i<6; i++)
		m_pvPoints[i].color = sf::Color::White;
}

void Quad::setPosition(sf::Vector2f pos)
{
	m_Position = pos;

	m_pvPoints[0].position = sf::Vector2f(m_Position.x - m_LocalBounds.width/2, m_Position.y - m_LocalBounds.height/2);
	m_pvPoints[1].position = sf::Vector2f(m_Position.x+m_LocalBounds.width/2, m_Position.y+m_LocalBounds.height/2);
	m_pvPoints[2].position = sf::Vector2f(m_Position.x+m_LocalBounds.width/2, m_Position.y - m_LocalBounds.height/2);
	m_pvPoints[3].position = sf::Vector2f(m_Position.x - m_LocalBounds.width/2, m_Position.y - m_LocalBounds.height/2);
	m_pvPoints[4].position = sf::Vector2f(m_Position.x - m_LocalBounds.width/2, m_Position.y+m_LocalBounds.height/2);
	m_pvPoints[5].position = sf::Vector2f(m_Position.x+m_LocalBounds.width/2, m_Position.y+m_LocalBounds.height/2);

	vector_rotate(m_Position, &m_pvPoints[0].position, angle_to_radians(m_RotationDegrees));
	vector_rotate(m_Position, &m_pvPoints[1].position, angle_to_radians(m_RotationDegrees));
	vector_rotate(m_Position, &m_pvPoints[2].position, angle_to_radians(m_RotationDegrees));
	vector_rotate(m_Position, &m_pvPoints[3].position, angle_to_radians(m_RotationDegrees));
	vector_rotate(m_Position, &m_pvPoints[4].position, angle_to_radians(m_RotationDegrees));
	vector_rotate(m_Position, &m_pvPoints[5].position, angle_to_radians(m_RotationDegrees));
}

void Quad::setSize(float width, float height)
{
	m_LocalBounds = sf::FloatRect(0.0f, 0.0f, width, height);
	setPosition(m_Position);
}

void Quad::setPoint(unsigned int index, Vector2f point)
{
	if (index > 3)
		return;

	m_pvPoints[index] = point;
}
sf::Vector2f Quad::getPoint(unsigned int index)
{
	if (index > 3)
		return sf::Vector2f(0.0f,0.0f);

	return m_pvPoints[index].position;
}
void Quad::setPointColor(unsigned int index, Color _color)
{
	if (index > 3)
		return;

	m_pvPoints[index].color = _color;
}
sf::Color Quad::getPointColor(unsigned int index)
{
	if (index > 3)
		return Color::White;

	return m_pvPoints[index].color;
}


void Quad::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	CGameCore *pCore = CGameCore::getInstance();

	target.draw(m_pvPoints, pCore->TextureManager()->get(m_TextureId));

	//Draw OutLine
	if (m_OutLine)
	{
		sf::VertexArray lines;
		for (size_t i=0; i<3; i++)
		{
			lines.append(sf::Vertex(m_pvPoints[i].position, m_OutLineColor));
			lines.append(sf::Vertex(m_pvPoints[i+1].position, m_OutLineColor));
		}
		lines.append(sf::Vertex(m_pvPoints[0].position, m_OutLineColor));
		lines.append(sf::Vertex(m_pvPoints[3].position, m_OutLineColor));

		target.draw(lines);
	}
}
