#include "Block.h"

#include "Chunk.h"

#pragma region Base Block
Block::Block(const int id, const sf::Color color)
{
	id_ = id;
	color_ = color;
}

void Block::operator=(const Block& other)
{
	id_ = other.id_;
	color_ = other.color_;
}

bool Block::operator==(const Block& other) const
{
	return id_ == other.id_;
}

bool Block::operator!=(const Block& other) const
{
	return id_ != other.id_;
}

sf::Color Block::GetColor() const
{
	return color_;
}

void Block::Update(Chunk& chunk, int x, int y)
{
}
#pragma endregion 

void SandBlock::Update(Chunk& chunk, const int x, const int y)
{
	if (*chunk.GetBlock(x, y - 1) == *blocks::air)
	{
		chunk.SetBlock(x, y, blocks::air);
		chunk.SetBlock(x, y - 1, this);
		return;
	}

	if (*chunk.GetBlock(x - 1, y - 1) == *blocks::air)
	{
		chunk.SetBlock(x, y, blocks::air);
		chunk.SetBlock(x - 1, y - 1, this);
		return;
	}

	if (*chunk.GetBlock(x + 1, y - 1) == *blocks::air)
	{
		chunk.SetBlock(x, y, blocks::air);
		chunk.SetBlock(x + 1, y - 1, this);
		return;
	}
}

void FluidBlock::Update(Chunk& chunk, const int x, const int y)
{
}
