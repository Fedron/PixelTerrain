#include "Chunk.h"
#include "World.h"

Chunk::Chunk(World* world, const int world_x, const int world_y) :
world_x_(world_x), world_y_(world_y),
blocks_(world->chunk_width_ * world->chunk_height_, blocks::kAir)
{
	world_ = world;
	
	texture_pixels_ = new sf::Uint8[world->chunk_width_ * world->chunk_height_ * 4];
	is_dirty_ = false;

	texture_.create(world->chunk_width_, world->chunk_height_);
	sprite_.setTexture(texture_);
	
	sprite_.setPosition(
		(world_x * world->chunk_width_),
		((world->num_chunks_y_ - 1) * world->chunk_height_) - (world_y * world->chunk_height_)
	);
}

Block Chunk::GetBlock(const int x, const int y) const
{
	if (x < 0 || x >= world_->chunk_width_ || y < 0 || y >= world_->world_height_)
		return blocks::kNull;

	return blocks_[x + y * world_->chunk_width_];
}

void Chunk::SetBlock(const int x, const int y, const Block block)
{
	if (x < 0 || x >= world_->chunk_width_ || y < 0 || y >= world_->world_height_)
		return;

	is_dirty_ = true;

	// Set actual block
	blocks_[x + y * world_->chunk_width_] = block;

	// Set texture pixel
	const int correct_y = world_->chunk_height_ - y - 1;
	const int pos = (x + correct_y * world_->chunk_width_) * 4;

	const sf::Color color = block.GetColor();
	texture_pixels_[pos] = color.r;
	texture_pixels_[pos + 1] = color.g;
	texture_pixels_[pos + 2] = color.b;
	texture_pixels_[pos + 3] = color.a;
}

void Chunk::Update()
{
}

void Chunk::Draw(sf::RenderWindow* window)
{
	if (is_dirty_)
	{
		is_dirty_ = false;
		texture_.update(texture_pixels_);
	}
	
	window->draw(sprite_);
}
