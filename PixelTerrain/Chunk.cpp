#include "Chunk.h"
#include "World.h"

#pragma warning(disable: 4244)

Chunk::Chunk(World& world, const int world_x, const int world_y) :
world_(world),
world_x_(world_x), world_y_(world_y),
current_blocks_(world.settings_.chunk_width* world.settings_.chunk_height, blocks::air),
next_blocks_(world.settings_.chunk_width* world.settings_.chunk_height, blocks::air),
vertices_(sf::Quads, world.settings_.chunk_width* world.settings_.chunk_height * 4)
{
}

Block* Chunk::GetBlock(const int x, const int y) const
{
	// Return null block if out of bounds
	if (x < 0 || x >= world_.settings_.chunk_width || y < 0 || y >= world_.settings_.chunk_height)
	{
		const int block_world_x = (world_x_ * world_.settings_.chunk_width) + x;
		const int block_world_y = (world_y_ * world_.settings_.chunk_height) + y;

		return world_.GetBlock(block_world_x, block_world_y);
	}

	// Return block
	return current_blocks_[x + y * world_.settings_.chunk_width];
}

void Chunk::SetBlock(const int x, const int y, Block* block)
{
	// Don't try setting a block if out of bounds
	if (x < 0 || x >= world_.settings_.chunk_width || y < 0 || y >= world_.settings_.chunk_height)
	{
		const int block_world_x = (world_x_ * world_.settings_.chunk_width) + x;
		const int block_world_y = (world_y_ * world_.settings_.chunk_height) + y;

		world_.SetBlock(block_world_x, block_world_y, block);
		return;
	}
	
	// Set the block
	next_blocks_[x + y * world_.settings_.chunk_width] = block;

	// Update vertices
	// Inverted-y to account for top-left being (0, 0) when drawing
	const int vertex_correct_y = world_.settings_.chunk_height - y - 1;
	// Gets the quad of the block in the VertexArray
	sf::Vertex* quad = &vertices_[(x + vertex_correct_y * world_.settings_.chunk_width) * 4];

	// Calculate the position of the chunk within the world
	const int world_offset_x = world_x_ * world_.settings_.chunk_width * world_.settings_.block_size;
	const int world_offset_y = ((world_.settings_.num_chunks_y - 1) * world_.settings_.chunk_width * world_.settings_.block_size) -
		(world_y_ * world_.settings_.chunk_width * world_.settings_.block_size);

	// Sets each vertex of the quad to the block's position and color
	quad[0].position = sf::Vector2f(
		world_offset_x + (x * world_.settings_.block_size),
		world_offset_y + (vertex_correct_y * world_.settings_.block_size));
	quad[0].color = block->GetColor();

	quad[1].position = sf::Vector2f(
		world_offset_x + ((x + 1) * world_.settings_.block_size),
		world_offset_y + (vertex_correct_y * world_.settings_.block_size));
	quad[1].color = block->GetColor();

	quad[2].position = sf::Vector2f(
		world_offset_x + ((x + 1) * world_.settings_.block_size),
		world_offset_y + ((vertex_correct_y + 1) * world_.settings_.block_size));
	quad[2].color = block->GetColor();

	quad[3].position = sf::Vector2f(
		world_offset_x + (x * world_.settings_.block_size),
		world_offset_y + ((vertex_correct_y + 1) * world_.settings_.block_size));
	quad[3].color = block->GetColor();
}

void Chunk::Update()
{
	// Run each block's update
	for (int y = 0; y < world_.settings_.chunk_height; y++)
	{
		for (int x = 0; x < world_.settings_.chunk_width; x++)
		{
			current_blocks_[x + y * world_.settings_.chunk_width]->Update(
				*this,
				x, y
			);
		}
	}
	
	UpdateBlocks();
}

void Chunk::UpdateBlocks()
{
	current_blocks_ = next_blocks_;
}

void Chunk::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	// Draw the VertexArray to the window
	target.draw(vertices_, states);
}
