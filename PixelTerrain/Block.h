#pragma once
#include "SFML/Graphics.hpp"

/**
 * Data structure for a block
 */
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

/**
 * Pre-defined blocks
 */
namespace blocks
{
	static Block null{ -1, sf::Color(0, 0, 0, 0) };
	static Block air{ 0, sf::Color(0, 0, 0, 0) };
	static Block dirt{ 1, sf::Color(114,84,40, 255) };
	static Block grass{ 2, sf::Color(124, 141, 76, 255) };
	static Block stone{ 3, sf::Color(124, 132, 133, 255) };
	static Block sand{ 4, sf::Color(229, 217, 194, 255) };
	static Block water{ 5, sf::Color(78, 182, 237, 255) };
}
