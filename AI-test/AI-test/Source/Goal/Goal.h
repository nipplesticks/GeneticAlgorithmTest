#pragma once
#include <SFML/Graphics.hpp>

class Goal : public sf::Drawable
{
private:
	sf::CircleShape m_spr;


public:
	Goal(float x, float y);

	float getRad() const;
	const sf::Vector2f & getPosition();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};