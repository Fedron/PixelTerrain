#include "World.h"

World::World(
	const int chunk_width, const int chunk_height,
	const int num_chunks_x, const int num_chunks_y,
	const int block_size,
	const int min_surface_level, const int max_surface_level,
	const int grass_layer_height, const int dirt_layer_height
) :
chunk_width_(chunk_width), chunk_height_(chunk_height),
num_chunks_x_(num_chunks_x), num_chunks_y_(num_chunks_y),
world_width_(chunk_width * num_chunks_x), world_height_(chunk_height * num_chunks_y),
block_size_(block_size),
min_surface_level_(min_surface_level), max_surface_level_(max_surface_level),
grass_layer_height_(grass_layer_height), dirt_layer_height_(dirt_layer_height)
{
	// Reserve memory space
	chunks_.reserve(num_chunks_x_ * num_chunks_y_);
	// Create empty chunks
	for (int chunk_y = 0; chunk_y < num_chunks_y_; chunk_y++)
	{
		for (int chunk_x = 0; chunk_x < num_chunks_x_; chunk_x++)
		{
			// Create a new chunk
			chunks_.emplace_back(new Chunk(*this, chunk_x, chunk_y));
		}
	}
}

void World::AddGenerationTask(void(*task)(World& world))
{
	// Adds the task to the end of the list
	generation_tasks_.push_back(task);
}

void World::Generate()
{
	// Get current time
	const auto start_time = std::chrono::high_resolution_clock::now();
	// Set seed to current time
	perlin_noise_.SetSeed(time(nullptr));

	// Execute all the generation tasks
	for (auto& generation_task : generation_tasks_)
	{
		(*generation_task)(*this);
	}

	// Calculate how long generation took
	const auto end_time = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cout << "Generation completed in " << duration.count() << "ms" << std::endl;
}

double World::GetNoise(const int x, const int y) const
{
	// Sample the perlin noise value at x, y
	return perlin_noise_.GetValue(x * 0.001, y * 0.001, 0);
}

Block World::GetBlock(const int x, const int y) const
{
	// Return null block if out of bounds
	if (x < 0 || x >= world_width_ || y < 0 || y >= world_height_)
		return blocks::null;

	// Calculate the chunk in which block is located
	const int chunk_x = x / chunk_width_;
	const int chunk_y = y / chunk_height_;

	// Get the chunk
	const Chunk* chunk = chunks_[chunk_x + chunk_y * num_chunks_x_];
	// Convert global coordinates to local chunk coordinates and get the block
	return chunk->GetBlock(x - (chunk_x * chunk_width_), y - (chunk_y * chunk_height_));
}

void World::SetBlock(const int x, const int y, const Block block)
{
	// Don't attempt to set a block if coordinates out of bounds
	if (x < 0 || x >= world_width_ || y < 0 || y >= world_height_)
		return;

	// Calculate the chunk coordinates
	const int chunk_x = x / chunk_width_;
	const int chunk_y = y / chunk_height_;

	// Get the chunk
	Chunk* chunk = chunks_[chunk_x + chunk_y * num_chunks_x_];
	// Convert global coordinates to local chunk coordinates and set the block
	chunk->SetBlock(x - (chunk_x * chunk_width_), y - (chunk_y * chunk_height_), block);
}

std::vector<Block> World::GetBlocks() const
{
	// Create a new vector to return filled with null blocks
	std::vector<Block> blocks(world_width_ * world_height_, blocks::null);

	for (int x = 0; x < world_width_; x++)
	{
		for (int y = 0; y < world_height_; y++)
		{
			// Set the block at (x, y) to the block at (x, y)
			blocks[x + y * world_width_] = GetBlock(x, y);
		}
	}

	// Return the new vector
	return blocks;
}

void World::SetBlocks(std::vector<Block> blocks)
{
	// Iterate through all (x, y) coordinates
	for (int x = 0; x < world_width_; x++)
	{
		for (int y = 0; y < world_height_; y++)
		{
			// Set the block in the world at (x, y) to the block in the new vector
			SetBlock(x, y, blocks[x + y * world_width_]);
		}
	}
}

void World::Draw(sf::RenderWindow& window)
{
	for (Chunk* chunk : chunks_)
	{
		// Draw the chunk to the window
		window.draw(*chunk);
	}
}
