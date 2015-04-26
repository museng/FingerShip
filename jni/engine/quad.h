/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_QUAD
#define H_QUAD

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

using namespace sf;

class Quad : public Drawable
{
public:
	Quad(Vector2f pos, float width, float height, int textId = -1);

	void setPoint(unsigned int index, Vector2f point);
	Vector2f getPoint(unsigned int index);
	void setPointColor(unsigned int index, Color _color);
	Color getPointColor(unsigned int index);

	void setOutLine(bool state)             { m_OutLine = state; }
	void setOutLineColor(Color _color)      { m_OutLineColor = _color; }

	void setPosition(sf::Vector2f pos);
	void setSize(float width, float height);
	void setRotation(float degrees)			{ m_RotationDegrees = degrees; }
	float getRotation() const 				{ return m_RotationDegrees; }

	VertexArray& getPoints() { return m_pvPoints; }

	void draw(RenderTarget& target, RenderStates states) const;

private:
	VertexArray m_pvPoints;
	Color m_OutLineColor;
	bool m_OutLine;
	int m_TextureId;
	FloatRect m_LocalBounds;
	Vector2f m_Position;
	float m_RotationDegrees;
};

#endif
