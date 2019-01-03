#include "Goal.h"

Goal::Goal(float x, float y)
{
	m_spr.setRadius(6.0f);
	m_spr.setFillColor(sf::Color::Red);
	m_spr.setOutlineThickness(-2.0f);
	m_spr.setOutlineColor(sf::Color::Black);
	m_spr.setOrigin(m_spr.getRadius(), m_spr.getRadius());
	m_spr.setPosition(x, y);
}

float Goal::getRad() const
{
	return m_spr.getRadius();
}

const sf::Vector2f & Goal::getPosition()
{
	return m_spr.getPosition();
}

void Goal::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_spr);
}
