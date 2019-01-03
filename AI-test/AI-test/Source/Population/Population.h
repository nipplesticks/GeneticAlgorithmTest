#pragma once
#include "../Dot/Dot.h"
#include "../Goal/Goal.h"

class Population : public sf::Drawable
{
private:
	std::vector<Dot> m_dots;
	std::vector<sf::RectangleShape> m_obsticles;
	Goal * m_targetGoal;
	int m_generation;
	int m_bestDot;
	int m_minStep;

public:
	Population(int size);
	virtual ~Population();

	void AddObsticle(const sf::RectangleShape & o);

	bool Update(float dt);
	void CalculateFitness();
	void NaturalSelection();
	void Mutate();
	int getMinStep() const;

	int getGeneration() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	float	_calculateFitnessSum();
	Dot	*	_selectParent(float fitnessSum);

};