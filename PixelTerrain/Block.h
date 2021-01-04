#pragma once
#include <SFML/Graphics/Color.hpp>

struct Block
{
	int id = 0;
	sf::Color color;

	void operator=(const Block& other)
	{
		id = other.id;
		color = other.color;
	}

	bool operator==(const Block& other) const
	{
		return id == other.id;
	}

	bool operator!=(const Block& other) const
	{
		return id != other.id;
	}
};

namespace blocks
{
	const Block kAir{ 0, sf::Color::Transparent };
	const Block kGrass{ 1, sf::Color::Green };
	const Block kDirt{ 2, sf::Color(156,96,67,255) };
	const Block kStone{ 3, sf::Color(150,150,150,255) };
	const Block kSand{ 3, sf::Color(242,209,107,255) };
	const Block kWater{ 5, sf::Color(78, 182, 237, 255) };
}