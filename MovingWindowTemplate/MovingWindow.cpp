#include "MovingWindow.h"

MovingWindow::MovingWindow(sf::Vector2u windowSize, sf::String title, sf::Uint32 style, unsigned int framerate) : window(sf::VideoMode(windowSize.x, windowSize.y), title, style), quads(sf::Quads), currentBitField(5, false),cellularRules(std::string("01100110"))
{
	// Max Framerate
	window.setFramerateLimit(framerate);
	// Set view to start in the point (0,0)
	sf::View view = window.getView();
	view.move(-static_cast<float>(windowSize.x) / 2, -static_cast<float>(windowSize.x) / 2);
	window.setView(view);

	// Set starting quad
	sf::Vertex quad[4];

	quad[0].color = sf::Color::Black;
	quad[1].color = sf::Color::Black;
	quad[2].color = sf::Color::Black;
	quad[3].color = sf::Color::Black;

	quad[0].position = sf::Vector2f(0.0f, 0.0f);
	quad[1].position = sf::Vector2f(0.0f, 5.0f);
	quad[2].position = sf::Vector2f(5.0f, 5.0f);
	quad[3].position = sf::Vector2f(5.0f, 0.0f);

	for (int i = 0; i < 4; i++)
	{
		quads.append(quad[i]);
	}
	// Set starting bit(middle) to true
	currentBitField[2] = true;
}

void MovingWindow::run()
{
	while (window.isOpen())
	{
		window.clear(sf::Color::White);
		draw();
		window.display();
		update();
		pollEvent();
	}
}

void MovingWindow::pollEvent()
{
	sf::Event e;
	while (window.pollEvent(e))
	{
		switch (e.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::Resized:
		{
			sf::View view = window.getView();
			view.setSize(static_cast<float>(e.size.width), static_cast<float>(e.size.height));
			view.zoom(zoomLevel);
			window.setView(view);
			break;
		}
		case sf::Event::MouseMoved:
			if (panning)
			{
				sf::View view = window.getView();
				sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(window) - this->panningAnchor);
				view.move(-1.0f * pos * zoomLevel);
				window.setView(view);
				panningAnchor = sf::Mouse::getPosition(window);
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (e.mouseButton.button == sf::Mouse::Middle)
			{
				panning = true;
				panningAnchor = sf::Mouse::getPosition(window);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (e.mouseButton.button == sf::Mouse::Middle)
			{
				panning = false;
			}
			break;
		case sf::Event::MouseWheelMoved:
		{
			sf::View view = window.getView();
			if (e.mouseWheel.delta < 0)
			{
				view.zoom(2.0f);
				zoomLevel *= 2;
			}
			else
			{
				view.zoom(0.5f);
				zoomLevel *= 0.5;
			}
			window.setView(view);
			break;
		}
		case sf::Event::KeyPressed:
		{
			// Bits flipping
			switch(e.key.code)
			{
			case sf::Keyboard::Num1:
				cellularRules[0].flip();
				break;
			case sf::Keyboard::Num2:
				cellularRules[1].flip();
				break;
			case sf::Keyboard::Num3:
				cellularRules[2].flip();
				break;
			case sf::Keyboard::Num4:
				cellularRules[3].flip();
				break;
			case sf::Keyboard::Num5:
				cellularRules[4].flip();
				break;
			case sf::Keyboard::Num6:
				cellularRules[5].flip();
				break;
			case sf::Keyboard::Num7:
				cellularRules[6].flip();
				break;
			case sf::Keyboard::Num8:
				cellularRules[7].flip();
				break;
			}
			break;
		}
		}
	}
}

void MovingWindow::update()
{
	std::vector<bool> newBitField(currentBitField.size() + 2, false);
	int middlePoint = static_cast<int>(newBitField.size() / 2);
	for (int i = 2; i < newBitField.size()-2; i++)
	{
		newBitField[i] = checkRules(currentBitField[i - 2], currentBitField[i-1], currentBitField[i]);
		if (newBitField[i])
		{
			
			// Create new quad
			sf::Vertex quad[4];

			quad[0].color = sf::Color::Black;
			quad[1].color = sf::Color::Black;
			quad[2].color = sf::Color::Black;
			quad[3].color = sf::Color::Black;

			int distance = std::abs(i - middlePoint);

			if (i < middlePoint)
				distance *= -1;

			quad[0].position = sf::Vector2f(0.0f + distance * 5.0f, 0.0f + rowNumber * 5.0f);
			quad[1].position = sf::Vector2f(0.0f + distance * 5.0f, 5.0f + rowNumber * 5.0f);
			quad[2].position = sf::Vector2f(5.0f + distance * 5.0f, 5.0f + rowNumber * 5.0f);
			quad[3].position = sf::Vector2f(5.0f + distance * 5.0f, 0.0f + rowNumber * 5.0f);

			for (int i = 0; i < 4; i++)
			{
				quads.append(quad[i]);
			}
		}
	}
	currentBitField = newBitField;
	rowNumber++;
}

void MovingWindow::draw()
{
	window.draw(quads);
}

std::pair<sf::Vector2f, sf::Vector2f> MovingWindow::getViewBox()
{
	sf::View view = window.getView();
	sf::Vector2f winStart = sf::Vector2f{ view.getCenter().x - view.getSize().x / 2,view.getCenter().y - view.getSize().y / 2 };
	sf::Vector2f winEnd = sf::Vector2f{ view.getCenter().x + view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2 };

	return { winStart,winEnd };
}

bool MovingWindow::checkRules(bool A, bool B, bool C)
{
	/////////Chang Returning values to get different structures////////////////////////////
	if (A && B && C)
	{
		return cellularRules[0];
	}
	else if (A && B && !C)
	{
		return cellularRules[1];
	}
	else if (A && !B && C)
	{
		return cellularRules[2];
	}
	else if (A && !B && !C)
	{
		return cellularRules[3];
	}
	else if (!A  && B && C)
	{
		return cellularRules[4];
	}
	else if (!A  && B && !C)
	{
		return cellularRules[5];
	}
	else if (!A && !B && C)
	{
		return cellularRules[6];
	}
	return cellularRules[7];
}
