#include "World.h"

World::World(
	const int chunk_width, const int chunk_height,
	const int num_chunks_x, const int num_chunks_y,
	const int block_size
) :
chunk_width_(chunk_width), chunk_height_(chunk_height),
num_chunks_x_(num_chunks_x), num_chunks_y_(num_chunks_y),
block_size_(block_size),
chunks_(num_chunks_x_ * num_chunks_y_, new Chunk(*this))
{
}

void World::Draw(sf::RenderWindow& window)
{
	for (Chunk* chunk : chunks_)
	{
		window.draw(*chunk);
	}
}
