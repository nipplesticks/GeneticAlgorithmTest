#include "Population/Population.h"
#include "External.h"
#include <time.h>
#include "HALP/Timer.h"
#include <iostream>
#include <DirectXMath.h>

sf::Vector2f GLOBAL::screenSize = {800.0f, 800.0f};

float GLOBAL::distBetween(const sf::Vector2f & p1, const sf::Vector2f & p2)
{
	DirectX::XMFLOAT2 _p1 = { p1.x, p1.y };
	DirectX::XMFLOAT2 _p2 = { p2.x, p2.y };
	DirectX::XMVECTOR _v1 = DirectX::XMLoadFloat2(&_p1);
	DirectX::XMVECTOR _v2 = DirectX::XMLoadFloat2(&_p2);
	DirectX::XMVECTOR _vDir = DirectX::XMVectorSubtract(_v2, _v1);

	return DirectX::XMVectorGetX(DirectX::XMVector2Length(_vDir));
}

int main()
{
	srand(static_cast<unsigned>(time(0)));
	sf::RenderWindow window(sf::VideoMode(GLOBAL::screenSize.x, GLOBAL::screenSize.y), "Test");
	
	int popSize = 2000;
	bool sleptLastFrame = false;
	Population myPop(popSize);

	sf::RectangleShape o;
	o.setPosition(GLOBAL::screenSize.x * 0.5f, GLOBAL::screenSize.y - 200.0f);
	sf::Vector2f oSize = sf::Vector2f(GLOBAL::screenSize.x * 0.8f, 20.0);
	o.setSize(oSize);
	o.setOrigin(oSize * 0.5f);
	o.setFillColor(sf::Color::Blue);
	o.setOutlineThickness(-3);
	o.setOutlineColor(sf::Color::Black);
	myPop.AddObsticle(o);

	o.setFillColor(sf::Color::Cyan);
	o.setPosition(GLOBAL::screenSize.x * 0.5f, 200.0f);
	myPop.AddObsticle(o);

	o.setPosition(GLOBAL::screenSize.x, GLOBAL::screenSize.y * 0.5f);
	o.setFillColor(sf::Color::Magenta);
	myPop.AddObsticle(o);
	
	o.setPosition(0.0f, GLOBAL::screenSize.y * 0.5f);
	myPop.AddObsticle(o);


	std::cout << "Generation: " << myPop.getGeneration() << std::endl;

	const double GAME_SPEED = 1000.0;


	double t = 0.0;
	double f = 1.0 / GAME_SPEED;
	double startTime = 0.0;
	bool firstRun = true;
	bool reset = false;
	
	Timer timer;
	timer.Start();
	
	while (window.isOpen())
	{
		t += timer.Stop();
		if (firstRun)
		{
			startTime = t;
			firstRun = false;
		}
		if (reset)
		{
			t = startTime;
			reset = false;
		}
		while (t >= f)
		{
			t -= f;
			if (!myPop.Update(0.5f))
			{
				reset = true;
				t = 0;
				myPop.CalculateFitness();
				myPop.NaturalSelection();
				myPop.Mutate();

				std::cout << "Generation: " << myPop.getGeneration() << std::endl;
				std::cout << "Best Last: " << myPop.getMinStep() << " steps\n";
			}
		}

		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
		}


		window.clear(sf::Color::White);
		window.draw(myPop);
		window.display();
	}


	return 0;
}