#include "Block.h"

#include "Chunk.h"

#pragma region Base block
Block::Block(const int id, const sf::Color color) :
id_(id), color_(color)
{
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

int Block::GetId() const
{
	return id_;
}

sf::Color Block::GetColor() const
{
	return color_;
}

void Block::OnUpdate(Chunk* chunk, int block_x, int block_y)
{
}
#pragma endregion

void FallableBlock::OnUpdate(Chunk* chunk, const int block_x, const int block_y)
{
	if (*chunk->GetBlock(block_x, block_y - 1) == *blocks::air)
	{
		chunk->SetBlock(block_x, block_y, blocks::air);
		chunk->SetBlock(block_x, block_y - 1, this);
	}
}

void FluidBlock::OnUpdate(Chunk* chunk, const int block_x, const int block_y)
{	
	if (*chunk->GetBlock(block_x, block_y - 1) == *blocks::air)
	{
		chunk->SetBlock(block_x, block_y, blocks::air);
		chunk->SetBlock(block_x, block_y - 1, this);
		return;
	}

	if (*chunk->GetBlock(block_x - 1, block_y - 1) == *blocks::air)
	{
		chunk->SetBlock(block_x, block_y, blocks::air);
		chunk->SetBlock(block_x - 1, block_y - 1, this);
		return;
	}

	if (*chunk->GetBlock(block_x + 1, block_y + 1) == *blocks::air)
	{
		chunk->SetBlock(block_x, block_y, blocks::air);
		chunk->SetBlock(block_x + 1, block_y + 1, this);
		return;
	}

	if (*chunk->GetBlock(block_x - 1, block_y) == *blocks::air)
	{
		chunk->SetBlock(block_x, block_y, blocks::air);
		chunk->SetBlock(block_x - 1, block_y, this);
		return;
	}

	if (*chunk->GetBlock(block_x + 1, block_y) == *blocks::air)
	{
		chunk->SetBlock(block_x, block_y, blocks::air);
		chunk->SetBlock(block_x + 1, block_y, this);
		return;
	}
}
