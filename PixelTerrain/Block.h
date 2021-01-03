#pragma once
#include <SFML/Graphics/Color.hpp>

struct Block
{
	const int id = 0;
	const sf::Color color;
};

namespace blocks
{
	const Block kAir{ 0, sf::Color::Transparent };
	const Block kGrass{ 1, sf::Color::Green };
	const Block kDirt{ 2, sf::Color(156,96,67,255) };
	const Block kStone{ 3, sf::Color(150,150,150,255) };
}