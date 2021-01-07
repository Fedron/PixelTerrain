#include "Chunk.h"

#include "World.h"

Chunk::Chunk(World& world, const int world_x, const int world_y) :
world_(world),
world_x_(world_x), world_y_(world_y),
blocks_(world.chunk_width_* world.chunk_height_, blocks::air),
vertices_(sf::Quads, world.chunk_width_* world.chunk_height_ * 4)
{
}

Block Chunk::GetBlock(const int x, const int y) const
{
	// Get block from surrounding chunks if x or y is out or bounds
	if (x < 0 || x >= world_.chunk_width_ || y < 0 || y >= world_.chunk_height_)
		return blocks::air;

	return blocks_[x + y * world_.chunk_width_];
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

	const int world_offset_x = world_x_ * world_.chunk_width_ * world_.block_size_;
	const int world_offset_y = world_y_ * world_.chunk_width_ * world_.block_size_;
	
	quad[0].position = sf::Vector2f(
		world_offset_x + (x * world_.block_size_),
		world_offset_y + (vertex_correct_y * world_.block_size_));
	quad[0].color = block.color;

	quad[1].position = sf::Vector2f(
		world_offset_x + ((x + 1) * world_.block_size_),
		world_offset_y + (vertex_correct_y * world_.block_size_));
	quad[1].color = block.color;

	quad[2].position = sf::Vector2f(
		world_offset_x + ((x + 1) * world_.block_size_),
		world_offset_y + ((vertex_correct_y + 1) * world_.block_size_));
	quad[2].color = block.color;

	quad[3].position = sf::Vector2f(
		world_offset_x + (x * world_.block_size_),
		world_offset_y + ((vertex_correct_y + 1) * world_.block_size_));
	quad[3].color = block.color;
}

void Chunk::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	target.draw(vertices_, states);
}
