#include "Chunk.h"
#include "World.h"

Chunk::Chunk(World& world, const int world_x, const int world_y) :
world_(world),
world_x_(world_x), world_y_(world_y),
blocks_(world.chunk_width_* world.chunk_height_, blocks::air),
vertices_(sf::Quads, world.chunk_width_* world.chunk_height_ * 4)
{
	/*font_.loadFromFile("font.ttf");
	text_.setFont(font_);
	text_.setString(std::to_string(world_x_) + ", " + std::to_string(world_y_));
	text_.setCharacterSize(18);
	text_.setFillColor(sf::Color::White);
	text_.setPosition(
		world_x_ * world_.chunk_width_ * world_.block_size_,
		((world_.num_chunks_y_ - 1) * world_.chunk_width_ * world_.block_size_) - world_y_ * world_.chunk_height_ * world_.block_size_
	);*/
}

Block Chunk::GetBlock(const int x, const int y) const
{
	// Return null block if out of bounds
	if (x < 0 || x >= world_.chunk_width_ || y < 0 || y >= world_.chunk_height_)
		return blocks::null;

	// Return block
	return blocks_[x + y * world_.chunk_width_];
}

void Chunk::SetBlock(const int x, const int y, const Block block)
{
	// Don't try setting a block if out of bounds
	if (x < 0 || x >= world_.chunk_width_ || y < 0 || y >= world_.chunk_height_)
		return;

	// Set the block
	blocks_[x + y * world_.chunk_width_] = block;

	// Update vertices
	// Inverted-y to account for top-left being (0, 0) when drawing
	const int vertex_correct_y = world_.chunk_height_ - y - 1;
	// Gets the quad of the block in the VertexArray
	sf::Vertex* quad = &vertices_[(x + vertex_correct_y * world_.chunk_width_) * 4];

	// Calculate the position of the chunk within the world
	const int world_offset_x = world_x_ * world_.chunk_width_ * world_.block_size_;
	const int world_offset_y = ((world_.num_chunks_y_ - 1) * world_.chunk_width_ * world_.block_size_) -
		(world_y_ * world_.chunk_width_ * world_.block_size_);

	// Sets each vertex of the quad to the block's position and color
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
	// Draw the VertexArray to the window
	target.draw(vertices_, states);
}
