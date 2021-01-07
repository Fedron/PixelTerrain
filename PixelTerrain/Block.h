#pragma once
#include "SFML/Graphics.hpp"

struct Block
{
	sf::Color color;
};

namespace blocks
{
	static Block air{ sf::Color(0, 0, 0, 0) };
	static Block dirt{ sf::Color(159, 96, 67, 255) };
}
