#include "Chunk.h"

#include "World.h"

Chunk::Chunk(World& world) :
world_(world),
blocks_(world.chunk_width_* world.chunk_height_, blocks::air),
vertices_(sf::Quads, world.chunk_width_* world.chunk_height_ * 4)
{
	SetBlock(0, 0, blocks::dirt);
	SetBlock(0, 1, blocks::dirt);
	SetBlock(0, 2, blocks::dirt);
	SetBlock(1, 0, blocks::dirt);
	SetBlock(1, 1, blocks::dirt);
	SetBlock(2, 0, blocks::dirt);
}

void Chunk::SetBlock(const int x, const int y, const Block block)
{
	if (x < 0 || x >= world_.chunk_width_ || y < 0 || y >= world_.chunk_height_)
		return;

	// Set the block
	blocks_[x + y * world_.chunk_width_] = block;

	// Update vertices
	const int vertex_correct_y = world_.chunk_height_ - y - 1;
	sf::Vertex* quad = &vertices_[(x + vertex_correct_y * world_.chunk_width_) * 4];
	
	quad[0].position = sf::Vector2f(x * world_.block_size_, vertex_correct_y * world_.block_size_);
	quad[0].color = block.color;

	quad[1].position = sf::Vector2f((x + 1) * world_.block_size_, vertex_correct_y * world_.block_size_);
	quad[1].color = block.color;

	quad[2].position = sf::Vector2f((x + 1) * world_.block_size_, (vertex_correct_y + 1) * world_.block_size_);
	quad[2].color = block.color;

	quad[3].position = sf::Vector2f(x * world_.block_size_, (vertex_correct_y + 1) * world_.block_size_);
	quad[3].color = block.color;
}

void Chunk::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	target.draw(vertices_, states);
}
