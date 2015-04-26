/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#include "quad.h"
#include "game_core.h"
#include "basic_functions.h"
#include <SFML/Graphics/Vertex.hpp>

const int Quad::NUM_VERTEX = 6; // Two Triangles

Quad::Quad(sf::Vector2f pos, float width, float height, int textId)
: Drawable(),
  m_vPoints(sf::Triangles, NUM_VERTEX)
{
	m_OutLine = false;
	m_OutLineColor = sf::Color::Red;
	m_Position = pos;
	m_RotationDegrees = 0.0f;

	setTextureId(textId);
	setSize(width, height);

	for (int i=0; i<NUM_VERTEX; i++)
		m_vPoints[i].color = sf::Color::White;
}
Quad::Quad(sf::Vector2f pos, int textId)
: Drawable(),
  m_vPoints(sf::Triangles, NUM_VERTEX)
{
	m_OutLine = false;
	m_OutLineColor = sf::Color::Red;
	m_Position = pos;
	m_RotationDegrees = 0.0f;

	setTextureId(textId);

	for (int i=0; i<NUM_VERTEX; i++)
		m_vPoints[i].color = sf::Color::White;
}

void Quad::setPosition(sf::Vector2f pos)
{
	m_Position = pos;
	setRotation(m_RotationDegrees);
}

void Quad::setRotation(float degrees)
{
	m_RotationDegrees = degrees;

	m_vPoints[0].position = sf::Vector2f(m_Position.x - m_LocalBounds.width/2, m_Position.y - m_LocalBounds.height/2);
	m_vPoints[1].position = sf::Vector2f(m_Position.x+m_LocalBounds.width/2, m_Position.y+m_LocalBounds.height/2);
	m_vPoints[2].position = sf::Vector2f(m_Position.x+m_LocalBounds.width/2, m_Position.y - m_LocalBounds.height/2);
	m_vPoints[3].position = sf::Vector2f(m_Position.x - m_LocalBounds.width/2, m_Position.y - m_LocalBounds.height/2);
	m_vPoints[4].position = sf::Vector2f(m_Position.x - m_LocalBounds.width/2, m_Position.y+m_LocalBounds.height/2);
	m_vPoints[5].position = sf::Vector2f(m_Position.x+m_LocalBounds.width/2, m_Position.y+m_LocalBounds.height/2);

	vector_rotate(m_Position, &m_vPoints[0].position, angle_to_radians(m_RotationDegrees));
	vector_rotate(m_Position, &m_vPoints[1].position, angle_to_radians(m_RotationDegrees));
	vector_rotate(m_Position, &m_vPoints[2].position, angle_to_radians(m_RotationDegrees));
	vector_rotate(m_Position, &m_vPoints[3].position, angle_to_radians(m_RotationDegrees));
	vector_rotate(m_Position, &m_vPoints[4].position, angle_to_radians(m_RotationDegrees));
	vector_rotate(m_Position, &m_vPoints[5].position, angle_to_radians(m_RotationDegrees));
}

void Quad::setSize(float width, float height)
{
	m_LocalBounds = sf::FloatRect(0.0f, 0.0f, width, height);
	setPosition(m_Position);
}

void Quad::setFillColor(sf::Color color)
{
	for (int i=0; i<NUM_VERTEX; m_vPoints[i++].color = color);
}

void Quad::setTextureId(int textId)
{
	m_TextureId = textId;

	CGameCore *pCore = CGameCore::getInstance();
	sf::Texture *pTexture = pCore->TextureManager()->get(m_TextureId);
	if (pTexture)
		setTextureRect(sf::IntRect(0, 0, (int)pTexture->getSize().x, (int)pTexture->getSize().y));
	else
		setTextureRect(sf::IntRect(0, 0, 1, 1));
}

void Quad::setTextureRect(const sf::IntRect &textureRect)
{
	m_TextureRect = textureRect;

	m_vPoints[0].texCoords = sf::Vector2f(m_TextureRect.left, m_TextureRect.top);
	m_vPoints[1].texCoords = sf::Vector2f(m_TextureRect.width, m_TextureRect.height);
	m_vPoints[2].texCoords = sf::Vector2f(m_TextureRect.width, m_TextureRect.top);
	m_vPoints[3].texCoords = sf::Vector2f(m_TextureRect.left, m_TextureRect.top);
	m_vPoints[4].texCoords = sf::Vector2f(m_TextureRect.left, m_TextureRect.height);
	m_vPoints[5].texCoords = sf::Vector2f(m_TextureRect.width, m_TextureRect.height);
}

void Quad::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	CGameCore *pCore = CGameCore::getInstance();

	states.texture = pCore->TextureManager()->get(m_TextureId);
	target.draw(m_vPoints, states);

	//Draw OutLine
	if (m_OutLine)
	{
		sf::VertexArray lines(sf::Lines, 8);

		lines[0] = sf::Vertex(m_vPoints[1].position, m_OutLineColor);
		lines[1] = sf::Vertex(m_vPoints[2].position, m_OutLineColor);

		lines[2] = sf::Vertex(m_vPoints[2].position, m_OutLineColor);
		lines[3] = sf::Vertex(m_vPoints[0].position, m_OutLineColor);

		lines[4] = sf::Vertex(m_vPoints[0].position, m_OutLineColor);
		lines[5] = sf::Vertex(m_vPoints[4].position, m_OutLineColor);

		lines[6] = sf::Vertex(m_vPoints[4].position, m_OutLineColor);
		lines[7] = sf::Vertex(m_vPoints[1].position, m_OutLineColor);

		target.draw(lines);
	}
}
