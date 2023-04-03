#pragma once
#include <SFML/Graphics.hpp>
#include <bitset>


class MovingWindow
{
public:
	MovingWindow(sf::Vector2u windowSize,sf::String title,sf::Uint32 style = sf::Style::Default,unsigned int framerate = 144);
	void run();
private:
	void pollEvent();
	void update();
	void draw();
	std::pair<sf::Vector2f, sf::Vector2f> getViewBox();
	bool checkRules(bool A, bool B, bool C);
private:
	sf::RenderWindow window;
	float zoomLevel{ 1.0f };
	// Panning
	sf::Vector2i panningAnchor{};
	bool panning{false};
	// Cellular elements
	sf::VertexArray quads;
	std::vector<bool> currentBitField;
	std::bitset<8> cellularRules;
	int rowNumber{ 1 };
};