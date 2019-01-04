#include "Dot.h"
#include "../External.h"
#include <iostream>
#include <DirectXMath.h>
long long int Dot::ID = 0;


Dot::Dot(float speed)
{
	m_id = ID++;
	m_spr.setFillColor(sf::Color::Black);
	m_spr.setRadius(3.0f);
	m_spr.setOrigin(m_spr.getRadius(), m_spr.getRadius());
	m_spr.setPosition(GLOBAL::screenSize.x * 0.5f, GLOBAL::screenSize.y - m_spr.getRadius() * 2.0f);

	m_accSpeed = speed;

	m_fitness = 0.0f;

	m_acc = { 0.0f, 0.0f };
	m_vel = { 0.0f, 0.0f };

	m_brain = new Brain(2000);
	m_alive = true;
	m_reachedGoal = false;

	m_closestYet = FLT_MAX;
}

Dot::Dot(const Dot & other)
{
	_copy(other);
}

Dot::~Dot()
{
	_cleanup();
}

void Dot::Reset()
{
	m_spr.setPosition(GLOBAL::screenSize.x * 0.5f, GLOBAL::screenSize.y - m_spr.getRadius() * 2.0f);
	m_fitness = 0.0f;
	m_acc = { 0.0f, 0.0f };
	m_vel = { 0.0f, 0.0f };
	m_alive = true;
	m_reachedGoal = false;
	m_spr.setFillColor(sf::Color::Black);
	m_closestYet = FLT_MAX;
	m_brain->Reset();
}

void Dot::setBest()
{
	m_spr.setFillColor(sf::Color::Green);
}

void Dot::Move(float dt)
{
	m_acc = m_brain->getNextDirection() * m_accSpeed;
	
	if (fabs(m_acc.x) < 0.001f && fabs(m_acc.y) < 0.001f)
	{
		m_vel = { 0.0f, 0.0f };
		m_alive = false;
	}

	m_vel += m_acc;

	_limit(m_vel, 5.0f);

	m_spr.move(m_vel * dt);
}

void Dot::Update(float dt)
{
	if (m_alive)
	{
		Move(dt);

		const sf::Vector2f & pos = m_spr.getPosition();
		float r = m_spr.getRadius();

		if (!m_alive	||						// Out of moves
			pos.x < r	||						// Col with left
			pos.y < r	||						// Col with top
			pos.x + r > GLOBAL::screenSize.x ||	// Col with right
			pos.y + r > GLOBAL::screenSize.y)	// Col with bot
		{
			m_alive = false;
			//std::cout << "Dot: " << m_id << " has Died!\n";
		}
	}
}

bool Dot::isAlive() const
{
	return m_alive;
}

void Dot::Kill()
{
	m_alive = false;
}

bool Dot::ReachedGoal()
{
	return m_reachedGoal;
}

void Dot::setGoalState(bool reached)
{
	m_reachedGoal = reached;
}

void Dot::CalculateFitness(const sf::Vector2f & pos)
{
	if (m_reachedGoal)
	{
		m_fitness = FLT_MAX / float(m_brain->getSteps() * m_brain->getSteps());
	}
	else
	{
		//float _l = std::min(GLOBAL::distBetween(m_spr.getPosition(), pos), m_closestYet );
		float _l = GLOBAL::distBetween(m_spr.getPosition(), pos);
		if (_l == 0.0f)
			m_fitness = FLT_MAX;
		else
		{
			m_fitness = 1.0 / (_l * _l);
		}
	}
}

float Dot::getFitness() const
{
	return m_fitness;
}

const sf::Vector2f & Dot::getPosition()
{
	return m_spr.getPosition();
}

void Dot::DistToGoal(float dist)
{
	if (dist < m_closestYet)
		m_closestYet = dist;
}

Dot Dot::GetChild()
{
	Dot child = *this;
	child.Reset();


	return child;
}

Brain * Dot::getBrain()
{
	return m_brain;
}

sf::FloatRect Dot::getGlobalBounds() const
{
	return m_spr.getGlobalBounds();
}

void Dot::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_spr);
}

Dot & Dot::operator=(const Dot & other)
{
	if (this != &other)
	{
		_cleanup();
		_copy(other);
	}
	return *this;
}

bool Dot::operator<(const Dot & other) const
{
	return m_fitness < other.m_fitness;
}

bool Dot::operator>(const Dot & other) const
{
	return !(*this < other);
}

float Dot::_clamp(float _val, float _min, float _max)
{
	float val = std::min(_val, _max);
	val = std::max(val, _min);
	return val;
}


void Dot::_limit(sf::Vector2f & target, float max)
{
	DirectX::XMFLOAT2 _t = { target.x, target.y };
	DirectX::XMVECTOR _v = DirectX::XMLoadFloat2(&_t);
	float _l = DirectX::XMVectorGetX(DirectX::XMVector2Length(_v));
	if (_l > max)
	{
		_v = DirectX::XMVector2Normalize(_v);
		_v = DirectX::XMVectorScale(_v, max);
		DirectX::XMStoreFloat2(&_t, _v);
		target = { _t.x, _t.y };
	}
}

void Dot::_copy(const Dot & other)
{
	m_id = other.m_id;
	m_vel = other.m_vel;
	m_acc = other.m_acc;
	m_accSpeed = other.m_accSpeed;
	m_alive = other.m_alive;
	m_reachedGoal = other.m_reachedGoal;
	m_spr = other.m_spr;
	m_brain = new Brain(*other.m_brain);
	m_fitness = other.m_fitness;
	m_closestYet = other.m_closestYet;
}

void Dot::_cleanup()
{
	delete m_brain;
	m_brain = nullptr;
}
