#include "MovingWindow.h"
#include <iostream>

int main()
{
	MovingWindow window(sf::Vector2u(500, 500),"Simulation",sf::Style::Default);

	window.run();

	return 0;
}