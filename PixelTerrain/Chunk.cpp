#include "Chunk.h"
#include "World.h"

#pragma warning(disable: 4244)

Chunk::Chunk(World& world, const int world_x, const int world_y) :
world_(world),
world_x_(world_x), world_y_(world_y),
blocks_(world.gen_settings_.chunk_width* world.gen_settings_.chunk_height, blocks::air),
vertices_(sf::Quads, world.gen_settings_.chunk_width* world.gen_settings_.chunk_height * 4)
{
	pixels_ = new sf::Uint8[world.gen_settings_.chunk_width * world.gen_settings_.chunk_height * 4];
	lights_.create(world.gen_settings_.chunk_width, world.gen_settings_.chunk_height);

	for (int x = 0; x < world.gen_settings_.chunk_width; x++)
	{
		for (int y = 0; y < world.gen_settings_.chunk_height; y++)
		{
			const int base = (x + y * world.gen_settings_.chunk_width) * 4;
			pixels_[base] = 100;
			pixels_[base + 1] = 100;
			pixels_[base + 2] = 100;
			pixels_[base + 3] = 255;
		}
	}
	
	lights_.update(pixels_);
}

Block Chunk::GetBlock(const int x, const int y) const
{
	// Return null block if out of bounds
	if (x < 0 || x >= world_.gen_settings_.chunk_width || y < 0 || y >= world_.gen_settings_.chunk_height)
		return blocks::null;

	// Return block
	return blocks_[x + y * world_.gen_settings_.chunk_width];
}

void Chunk::SetBlock(const int x, const int y, const Block block)
{
	// Don't try setting a block if out of bounds
	if (x < 0 || x >= world_.gen_settings_.chunk_width || y < 0 || y >= world_.gen_settings_.chunk_height)
		return;
	
	// Set the block
	blocks_[x + y * world_.gen_settings_.chunk_width] = block;

	// Update vertices
	// Inverted-y to account for top-left being (0, 0) when drawing
	const int vertex_correct_y = world_.gen_settings_.chunk_height - y - 1;
	// Gets the quad of the block in the VertexArray
	sf::Vertex* quad = &vertices_[(x + vertex_correct_y * world_.gen_settings_.chunk_width) * 4];

	// Calculate the position of the chunk within the world
	const int world_offset_x = world_x_ * world_.gen_settings_.chunk_width * world_.gen_settings_.block_size;
	const int world_offset_y = ((world_.gen_settings_.num_chunks_y - 1) * world_.gen_settings_.chunk_width * world_.gen_settings_.block_size) -
		(world_y_ * world_.gen_settings_.chunk_width * world_.gen_settings_.block_size);

	// Sets each vertex of the quad to the block's position and color
	quad[0].position = sf::Vector2f(
		world_offset_x + (x * world_.gen_settings_.block_size),
		world_offset_y + (vertex_correct_y * world_.gen_settings_.block_size));
	quad[0].color = block.color;
	quad[0].texCoords = sf::Vector2f(0, 0);

	quad[1].position = sf::Vector2f(
		world_offset_x + ((x + 1) * world_.gen_settings_.block_size),
		world_offset_y + (vertex_correct_y * world_.gen_settings_.block_size));
	quad[1].color = block.color;
	quad[1].texCoords = sf::Vector2f(0, 0);

	quad[2].position = sf::Vector2f(
		world_offset_x + ((x + 1) * world_.gen_settings_.block_size),
		world_offset_y + ((vertex_correct_y + 1) * world_.gen_settings_.block_size));
	quad[2].color = block.color;
	quad[2].texCoords = sf::Vector2f(0, 0);

	quad[3].position = sf::Vector2f(
		world_offset_x + (x * world_.gen_settings_.block_size),
		world_offset_y + ((vertex_correct_y + 1) * world_.gen_settings_.block_size));
	quad[3].color = block.color;
	quad[3].texCoords = sf::Vector2f(0, 0);
}

void Chunk::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	world_.GetShader().setUniform("lightness", lights_);
	states.shader = &world_.GetShader();
	
	// Draw the VertexArray to the window
	target.draw(vertices_, states);
}
