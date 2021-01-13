#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"

class Chunk;

/**
 * Data structure for a block
 */
class Block 
{
public:
	Block(int id, sf::Color color);
	virtual ~Block() = default;

	void operator=(const Block& other);
	bool operator==(const Block& other) const;
	bool operator!=(const Block& other) const;

	sf::Color GetColor() const;

	virtual void Update(Chunk& chunk, int x, int y);

protected:
	// Unique ID of the block
	int id_;
	// Color of the block
	sf::Color color_;
};

/**
 * Implements special update function to allow sand to fall
 */
class SandBlock final : public Block
{
public:
	SandBlock(const int id, const sf::Color color) :
	Block(id, color) {}

	void Update(Chunk& chunk, int x, int y) override;
};

class FluidBlock final : public Block
{
public:
	FluidBlock(const int id, const sf::Color color) :
		Block(id, color) {}

	void Update(Chunk& chunk, int x, int y) override;
	
};

/**
 * Pre-defined blocks
 */
namespace blocks
{
	static Block* null = new Block(-1, sf::Color(0, 0, 0, 0));
	static Block* air = new Block(0, sf::Color(0, 0, 0, 0));
	static Block* dirt = new Block(1, sf::Color(114,84,40, 255));
	static Block* grass = new Block(2, sf::Color(124, 141, 76, 255));
	static Block* stone = new Block(3, sf::Color(124, 132, 133, 255));
	static SandBlock* sand = new SandBlock(4, sf::Color(229, 217, 194, 255));
	static FluidBlock* water = new FluidBlock(5, sf::Color(78, 182, 237, 255));
	static Block* wood = new Block(6, sf::Color(139, 90, 43, 255));
	static Block* leaf = new Block(7, sf::Color(5, 144, 51, 255));
}
