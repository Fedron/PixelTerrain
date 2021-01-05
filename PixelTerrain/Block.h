#pragma once
#include <SFML/Graphics/Color.hpp>

class Chunk;

class Block
{
public:
	Block(int id, sf::Color color);
	virtual ~Block() = default;
	
	void operator=(const Block& other);
	bool operator==(const Block& other) const;
	bool operator!=(const Block& other) const;

public:
	int GetId() const;
	sf::Color GetColor() const;

	virtual void OnUpdate(Chunk* chunk, int block_x, int block_y);

private:
	int id_;
	sf::Color color_;
};

class FallableBlock final : public Block
{
public:
	FallableBlock(const int id, const sf::Color color) : Block(id, color)
	{}
	virtual ~FallableBlock() override = default;
	
	void OnUpdate(Chunk* chunk, int block_x, int block_y) override;
};

class FluidBlock final : public Block
{
public:
	FluidBlock(const int id, const sf::Color color) : Block(id, color)
	{}
	virtual ~FluidBlock() override = default;

	void OnUpdate(Chunk* chunk, int block_x, int block_y) override;
};

namespace blocks
{
	static Block* null = new Block(-1, sf::Color::Red);
	static Block* air = new Block(0, sf::Color::Transparent);
	static Block* grass = new Block(1, sf::Color::Green);
	static Block* dirt = new Block(2, sf::Color(156,96,67,255));
	static Block* stone = new Block(3, sf::Color(150,150,150,255));
	static FallableBlock* sand = new FallableBlock(4, sf::Color(242,209,107,255));
	static FluidBlock* water = new FluidBlock(5, sf::Color(78, 182, 237, 255));
}