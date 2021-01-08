#pragma once
#include "SFML/Graphics.hpp"

struct Block
{
	// Unique ID of the block
	int id;
	// Color of the block
	sf::Color color;

	void operator=(const Block& other);
	bool operator==(const Block& other) const;
	bool operator!=(const Block& other) const;
};
 
namespace blocks
{
	static Block null{ -1, sf::Color(0, 0, 0, 0) };
	static Block air{ 0, sf::Color(0, 0, 0, 0) };
	static Block dirt{ 1, sf::Color(159, 96, 67, 255) };
	static Block grass{ 2, sf::Color(124, 252, 0, 255) };
	static Block stone{ 3, sf::Color(150, 150, 150, 255) };
	static Block sand{ 4, sf::Color(242,209,107, 255) };
	static Block water{ 5, sf::Color(78, 182, 237, 255) };
}
