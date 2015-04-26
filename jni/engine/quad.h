/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_QUAD
#define H_QUAD

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Quad : public sf::Drawable
{
public:
	Quad(sf::Vector2f pos, float width, float height, int textId = -1);
	Quad(sf::Vector2f pos, int textId = -1);

	static const int NUM_VERTEX;

	void setOutLine(bool state) { m_OutLine = state; }
	void setOutLineColor(sf::Color color) { m_OutLineColor = color; }
	void setFillColor(sf::Color color);

	void setPosition(sf::Vector2f pos);
	void setSize(float width, float height);
	void setRotation(float degrees);
	float getRotation() const { return m_RotationDegrees; }

	void setTextureId(int textId);
	int getTextureId() const { return m_TextureId; }
	void setTextureRect(const sf::IntRect &textureRect);

	sf::FloatRect& getLocalBounds() { return m_LocalBounds; }
	sf::FloatRect getGlobalBounds() const { return sf::FloatRect(m_Position.x-m_LocalBounds.width/2, m_Position.y-m_LocalBounds.height/2, m_LocalBounds.width, m_LocalBounds.height); }

	sf::VertexArray& getPoints() { return m_vPoints; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::VertexArray m_vPoints;
	sf::IntRect m_TextureRect;
	sf::Color m_OutLineColor;
	bool m_OutLine;
	int m_TextureId;
	sf::FloatRect m_LocalBounds;
	sf::Vector2f m_Position;
	float m_RotationDegrees;
};

#endif
