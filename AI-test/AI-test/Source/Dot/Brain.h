#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
class Brain
{
private:
	std::vector< sf::Vector2f > m_directions;
	int m_step = 0;

public:
	Brain(int size);
	Brain(const Brain & other);
	virtual ~Brain();

	void Reset();
	void Mutate();

	int getSteps() const;

	sf::Vector2f getNextDirection();
	Brain & operator=(const Brain & other);
private:
	void _randomize();

	void _copy(const Brain & other);
	void _cleanup();

};