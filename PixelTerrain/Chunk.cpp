#include "Chunk.h"
#include "World.h"

#pragma warning(disable: 4244)

Chunk::Chunk(World& world, const int world_x, const int world_y) :
world_(world),
world_x_(world_x), world_y_(world_y),
blocks_(world.settings_.chunk_width* world.settings_.chunk_height, blocks::air),
vertices_(sf::Quads, world.settings_.chunk_width* world.settings_.chunk_height * 4)
{
	// Setups the coord text
	coords_text_.setFont(world_.GetFont());
	coords_text_.setString(std::to_string(world_x_) + ", " + std::to_string(world_y_));
	coords_text_.setCharacterSize(12);
	coords_text_.setFillColor(sf::Color::White);

	// Sets the position of the coord text to the top-left of the chunk
	coords_text_.setPosition(
		(world_x_ * world_.settings_.chunk_width * world_.settings_.block_size) + 2,
		(((world_.settings_.num_chunks_y - 1) * world_.settings_.chunk_width * world_.settings_.block_size) - world_y_ * world_.settings_.chunk_height * world_.settings_.block_size) + 2
	);

	// Setups the update text
	update_text_.setFont(world_.GetFont());
	update_text_.setCharacterSize(10);
	UpdateUpdateText();

	// Sets the position of the update text to the top-right
	update_text_.setPosition(
		(world_x_ * world_.settings_.chunk_width * world_.settings_.block_size) + world_.settings_.chunk_width - 2,
		(((world_.settings_.num_chunks_y - 1) * world_.settings_.chunk_width * world_.settings_.block_size) - world_y_ * world_.settings_.chunk_height * world_.settings_.block_size) + 2
	);
}

Block Chunk::GetBlock(const int x, const int y) const
{
	// Return null block if out of bounds
	if (x < 0 || x >= world_.settings_.chunk_width || y < 0 || y >= world_.settings_.chunk_height)
		return blocks::null;

	// Return block
	return blocks_[x + y * world_.settings_.chunk_width];
}

void Chunk::SetBlock(const int x, const int y, const Block block)
{
	// Don't try setting a block if out of bounds
	if (x < 0 || x >= world_.settings_.chunk_width || y < 0 || y >= world_.settings_.chunk_height)
		return;

	// Set the block
	blocks_[x + y * world_.settings_.chunk_width] = block;

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
	quad[0].color = block.color;

	quad[1].position = sf::Vector2f(
		world_offset_x + ((x + 1) * world_.settings_.block_size),
		world_offset_y + (vertex_correct_y * world_.settings_.block_size));
	quad[1].color = block.color;

	quad[2].position = sf::Vector2f(
		world_offset_x + ((x + 1) * world_.settings_.block_size),
		world_offset_y + ((vertex_correct_y + 1) * world_.settings_.block_size));
	quad[2].color = block.color;

	quad[3].position = sf::Vector2f(
		world_offset_x + (x * world_.settings_.block_size),
		world_offset_y + ((vertex_correct_y + 1) * world_.settings_.block_size));
	quad[3].color = block.color;
}

void Chunk::UpdateUpdateText()
{
	if (updating_)
	{
		update_text_.setString("U");
		update_text_.setFillColor(sf::Color::Green);
	} else
	{
		update_text_.setString("S");
		update_text_.setFillColor(sf::Color::Red);
	}
}

void Chunk::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	// Draw the VertexArray to the window
	target.draw(vertices_, states);

	// Draw the chunk coordinates
	if (world_.debug_mode_)
	{
		target.draw(coords_text_);
		target.draw(update_text_);
	}
}
