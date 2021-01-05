#pragma once
#include <SFML/Graphics/Color.hpp>

class Terrain;

class Block
{
public:
	Block(int id, sf::Color color);
	~Block() = default;
	
	void operator=(const Block& other);
	bool operator==(const Block& other) const;
	bool operator!=(const Block& other) const;

public:
	sf::Color GetColor() const;

private:
	int id_;
	sf::Color color_;
};

namespace blocks
{
	const Block kNull{ -1, sf::Color::Red };
	const Block kAir{ 0, sf::Color::Transparent };
	const Block kGrass{ 1, sf::Color::Green };
	const Block kDirt{ 2, sf::Color(156,96,67,255) };
	const Block kStone{ 3, sf::Color(150,150,150,255) };
	const Block kSand{ 4, sf::Color(242,209,107,255) };
	const Block kWater{ 5, sf::Color(78, 182, 237, 255) };
}