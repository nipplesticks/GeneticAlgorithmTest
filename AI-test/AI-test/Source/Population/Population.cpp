#include "Population.h"
#include "../External.h"
#include <algorithm>

Population::Population(int size)
{
	m_minStep = INT_MAX;
	m_generation = 0;
	m_dots = std::vector<Dot>(size);
	m_targetGoal = new Goal(GLOBAL::screenSize.x * 0.5f, 6.0f);
	m_bestDot = -1;
}

Population::~Population()
{
	delete m_targetGoal;
	m_targetGoal = nullptr;
}

void Population::AddObsticle(const sf::RectangleShape & o)
{
	m_obsticles.push_back(o);
}

bool Population::Update(float dt)
{

	bool canContinueUpdate = false;
	if (m_dots.size() > 1000)
	{
		static const int STEP = 1000;
		static const int START = 0;

		static int target = 0;
		static int end = 1000;
		while (!canContinueUpdate)
		{
			bool swap = false;
			if (end <= target)
			{
				target = START;
				swap = true;
			}

			for (target; target < end; target++)
			{
				Dot & d = m_dots[target];
				if (d.getBrain()->getSteps() > m_minStep)
					d.Kill();
				else
				{
					for (auto & o : m_obsticles)
					{
						if (o.getGlobalBounds().intersects(d.getGlobalBounds()))
						{
							d.Kill();
							break;
						}
					}
				}

				if (d.isAlive() && !d.ReachedGoal())
				{
					d.Update(dt);
					float dist = GLOBAL::distBetween(d.getPosition(), m_targetGoal->getPosition());
					if (dist < 6.0f)
						d.setGoalState(true);
					else
					{
						d.DistToGoal(dist);
						canContinueUpdate = true;
					}
				}
			}

			if (end == m_dots.size())
			{
				end = STEP;
			}
			else
			{
				end += STEP;
				if (end > m_dots.size())
				{
					end = m_dots.size();
				}
			}
			if (swap && !canContinueUpdate)
			{
				return false;
			}

		}
		
	}
	else
	{
		for (auto & d : m_dots)
		{
			if (d.getBrain()->getSteps() > m_minStep)
				d.Kill();
			else
			{
				for (auto & o : m_obsticles)
				{
					if (o.getGlobalBounds().intersects(d.getGlobalBounds()))
					{
						d.Kill();
						break;
					}
				}
			}

			if (d.isAlive() && !d.ReachedGoal())
			{
				d.Update(dt);
				float dist = GLOBAL::distBetween(d.getPosition(), m_targetGoal->getPosition());
				if (dist < 6.0f)
					d.setGoalState(true);
				else
				{
					d.DistToGoal(dist);
					canContinueUpdate = true;
				}
			}
		}
	}

	return canContinueUpdate;
}

void Population::CalculateFitness()
{
	int counter = 0;
	float fitness = -1.0f;

	for (auto & d : m_dots)
	{
		d.CalculateFitness(m_targetGoal->getPosition());
		if (d.getFitness() > fitness)
		{
			m_bestDot = counter;
			fitness = d.getFitness();
		}
		counter++;
	}
}

void Population::NaturalSelection()
{
	m_generation++;
	std::vector<Dot> newDots(m_dots.size());
	float fitnessSum = _calculateFitnessSum();

	if (m_bestDot != -1)
	{
		newDots[m_dots.size() - 1] = m_dots[m_bestDot].GetChild();
		newDots[m_dots.size() - 1].setBest();
		if (m_dots[m_bestDot].ReachedGoal())
			m_minStep = m_dots[m_bestDot].getBrain()->getSteps();
	}

	for (int i = 0; i < newDots.size() - (m_bestDot != -1); i++)
	{
		Dot * parent = _selectParent(fitnessSum);

		if (parent)
			newDots[i] = parent->GetChild();
		else
			newDots[i] = m_dots[m_bestDot].GetChild();
	}

	m_dots.clear();
	m_dots = newDots;
}

void Population::Mutate()
{
	for (int i = 0; i < m_dots.size() - (m_bestDot != -1); i++)
	{
		m_dots[i].getBrain()->Mutate();
	}
}

int Population::getMinStep() const
{
	return m_minStep;
}

int Population::getGeneration() const
{
	return m_generation;
}

void Population::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*m_targetGoal);
	
	for (auto & o : m_obsticles)
		target.draw(o);

	/*if (m_bestDot != -1)
		target.draw(m_dots[m_bestDot]);*/

	for (auto & d : m_dots)
	{
		target.draw(d);
	}
}

float Population::_calculateFitnessSum()
{
	float fitnessSum = 0;

	for (auto & d : m_dots)
	{
		fitnessSum += d.getFitness();
	}

	return fitnessSum;
}

Dot * Population::_selectParent(float fitnessSum)
{
	double precentage = double(rand()) / double(RAND_MAX);
	float random = precentage * fitnessSum;
	float runningSum = 0;
	for (int i = 0; i < m_dots.size(); i++)
	{
		runningSum += m_dots[i].getFitness();
		if (runningSum >= random)
			return &m_dots[i];
	}

	return nullptr;
}
