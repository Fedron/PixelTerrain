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
	int max_fall = 0;
	for (int i = 1; i <= fall_speed_; i++)
	{
		if (*chunk->GetBlock(block_x, block_y - i) == *blocks::air)
			max_fall++;
		else
			break;
	}

	if (max_fall == 0) return;

	chunk->SetBlock(block_x, block_y, blocks::air);
	chunk->SetBlock(block_x, block_y - max_fall, this);
}

void FluidBlock::OnUpdate(Chunk* chunk, const int block_x, const int block_y)
{
}
