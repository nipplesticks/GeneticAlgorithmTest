#include "Brain.h"

Brain::Brain(int size)
{
	m_directions = std::vector<sf::Vector2f>(size);
	_randomize();
}

Brain::Brain(const Brain & other)
{
	_copy(other);
}

Brain::~Brain()
{
	_cleanup();
}

void Brain::Reset()
{
	m_step = 0;
}

void Brain::Mutate()
{
	for (auto & d : m_directions)
	{
		bool mutate = (rand() % 50) == 0;

		if (mutate)
		{
			float dir = double(rand());
			d.x = cos(dir);
			d.y = sin(dir);
		}
	}
}

int Brain::getSteps() const
{
	return m_step;
}

sf::Vector2f Brain::getNextDirection()
{
	if (m_step < m_directions.size())
		return m_directions[m_step++];
	else
		return sf::Vector2f(0.0f, 0.0f);		
}

Brain & Brain::operator=(const Brain & other)
{
	if (this != &other)
	{
		_cleanup();
		_copy(other);
	}

	return *this;
}

void Brain::_randomize()
{
	for (auto & d : m_directions)
	{
		float dir = double(rand());

		d.x = cos(dir);
		d.y = sin(dir);
	}
}

void Brain::_copy(const Brain & other)
{
	m_step = other.m_step;
	m_directions = other.m_directions;
}

void Brain::_cleanup()
{
	m_directions.clear();
}
