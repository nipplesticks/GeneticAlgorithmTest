#pragma once
#include <SFML/Graphics.hpp>
#include "Brain.h"

class Dot : public sf::Drawable
{
private:
	static long long int ID;

private:
	long long int m_id;

	sf::Vector2f m_vel;
	sf::Vector2f m_acc;
	
	float m_accSpeed;
	bool m_alive;
	bool m_reachedGoal;
	
	sf::CircleShape m_spr;

	Brain * m_brain = nullptr;
	float	m_fitness;
	float	m_closestYet;

public:
	Dot(float speed = 1.0f);
	Dot(const Dot & other);



	virtual ~Dot();

	void Reset();

	void setBest();

	void Move(float dt);
	void Update(float dt);
	bool isAlive() const;
	void Kill();
	bool ReachedGoal();
	void setGoalState(bool reached);

	void CalculateFitness(const sf::Vector2f & GoalPos);
	float getFitness() const;
	const sf::Vector2f & getPosition();
	void DistToGoal(float dist);


	Dot GetChild();
	Brain * getBrain();

	sf::FloatRect getGlobalBounds() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Dot & operator=(const Dot & other);
	bool operator<(const Dot & other) const;
	bool operator>(const Dot & other) const;

private:
	float _clamp(float _val, float _min, float _max);
	void _limit(sf::Vector2f & target, float max);

	void _copy(const Dot & other);
	void _cleanup();

};