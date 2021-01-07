#include "World.h"

World::World(
	const int chunk_width, const int chunk_height,
	const int num_chunks_x, const int num_chunks_y,
	const int block_size
) :
chunk_width_(chunk_width), chunk_height_(chunk_height),
num_chunks_x_(num_chunks_x), num_chunks_y_(num_chunks_y),
world_width_(chunk_width * num_chunks_x), world_height_(chunk_height * num_chunks_y),
block_size_(block_size)
{
	chunks_.reserve(num_chunks_x_ * num_chunks_y_);
	// Create empty chunks
	for (int chunk_x = 0; chunk_x < num_chunks_x_; chunk_x++)
	{
		for (int chunk_y = 0; chunk_y < num_chunks_y_; chunk_y++)
		{
			chunks_.emplace_back(new Chunk(*this, chunk_x, chunk_y));
		}
	}
}

void World::AddGenerationStep(void(*step)(World& world))
{
	generation_steps_.push_back(step);
}

void World::Generate()
{
	// TODO: Add time taken
	
	for (auto& generation_step : generation_steps_)
	{
		(*generation_step)(*this);
	}
}

void World::SetBlock(const int x, const int y, const Block block)
{
	if (x < 0 || x >= world_width_ || y < 0 || y >= world_height_)
		return;

	const int chunk_x = x / chunk_width_;
	const int chunk_y = y / chunk_height_;

	Chunk* chunk = chunks_[chunk_x + chunk_y * num_chunks_x_];
	chunk->SetBlock(x - (chunk_x * chunk_width_), y - (chunk_y * chunk_height_), block);
}

void World::Draw(sf::RenderWindow& window)
{
	for (Chunk* chunk : chunks_)
	{
		window.draw(*chunk);
	}
}
