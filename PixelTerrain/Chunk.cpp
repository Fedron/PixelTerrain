#include "Chunk.h"
#include "World.h"

Chunk::Chunk(World* world, const int world_x, const int world_y) :
world_x_(world_x), world_y_(world_y),
blocks_(world->chunk_width_ * world->chunk_height_, blocks::air)
{
	world_ = world;
	
	texture_pixels_ = new sf::Uint8[world->chunk_width_ * world->chunk_height_ * 4];
	is_dirty_ = false;

	texture_.create(world->chunk_width_, world->chunk_height_);
	sprite_.setTexture(texture_);
	
	sprite_.setPosition(
		world_x * world->chunk_width_,
		((world->num_chunks_y_ - 1) * world->chunk_height_) - (world_y * world->chunk_height_)
	);
}

Block* Chunk::GetBlock(const int x, const int y) const
{
	// Get block from surrounding chunks if x or y is out or bounds
	if (x < 0 || x >= world_->chunk_width_ || y < 0 || y >= world_->chunk_height_)
	{
		const int global_x = (world_x_ * world_->chunk_width_) + x;
		const int global_y = (world_y_ * world_->chunk_height_) + y;
		
		return world_->GetBlock(global_x, global_y);
	}

	return blocks_[x + y * world_->chunk_width_];
}

void Chunk::SetBlock(const int x, const int y, Block* block)
{
	// Set block in surrounding chunks if x or y is out or bounds
	if (x < 0 || x >= world_->chunk_width_ || y < 0 || y >= world_->chunk_height_)
	{
		const int global_x = (world_x_ * world_->chunk_width_) + x;
		const int global_y = (world_y_ * world_->chunk_height_) + y;

		world_->SetBlock(global_x, global_y, block);
		return;
	}

	is_dirty_ = true;

	// Set actual block
	blocks_[x + y * world_->chunk_width_] = block;

	// Set texture pixel
	const int correct_y = world_->chunk_height_ - y - 1;
	const int pos = (x + correct_y * world_->chunk_width_) * 4;

	const sf::Color color = block->GetColor();
	texture_pixels_[pos] = color.r;
	texture_pixels_[pos + 1] = color.g;
	texture_pixels_[pos + 2] = color.b;
	texture_pixels_[pos + 3] = color.a;
}

void Chunk::Update()
{
	/*
	 * TODO: Only update a chunk if it needs to be
	 * i.e it has sand or other gravity affected blocks
	 * an entirely static block filled chunk doesn't need
	 * to update it's blocks
	 */

	/*
	 * TODO: Cleanup and delete all pointers
	 * World now stores pointers to chunks
	 * Chunks now store pointers to blocks
	 */
	for (int x = 0; x < world_->chunk_width_; x++)
	{
		for (int y = 0; y < world_->chunk_height_; y++)
		{
			GetBlock(x, y)->OnUpdate(this, x, y);
		}
	}
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
