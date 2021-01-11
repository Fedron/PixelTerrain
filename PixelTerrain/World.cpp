#include "World.h"

World::World(const WorldSettings gen_settings, const int render_range) :
render_range_(render_range)
{
	gen_settings_ = gen_settings;
}

World::~World()
{
	for (Chunk* chunk : chunks_)
	{
		delete chunk;
	}

	delete[] lighting_pixels_;
}

void World::ResetChunks()
{
	// Delete previous chunk pointers
	for (Chunk* chunk : chunks_)
	{
		delete chunk;
	}
	chunks_.clear();
	
	// Reserve memory space
	chunks_.reserve(gen_settings_.num_chunks_x * gen_settings_.num_chunks_y);
	// Create empty chunks
	for (int chunk_y = 0; chunk_y < gen_settings_.num_chunks_y; chunk_y++)
	{
		for (int chunk_x = 0; chunk_x < gen_settings_.num_chunks_x; chunk_x++)
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
	// Recalculate necessary values
	world_width_ = gen_settings_.num_chunks_x * gen_settings_.chunk_width;
	world_height_ = gen_settings_.num_chunks_y * gen_settings_.chunk_height;

	min_surface_level_ = gen_settings_.min_surface_level;
	max_surface_level_ = gen_settings_.max_surface_level;

	// Update lighting texture
	delete lighting_pixels_;
	lighting_pixels_ = new sf::Uint8[world_width_ * world_height_ * 4];
	lighting_tex_.create(world_width_, world_height_);
	lighting_.setTexture(lighting_tex_);
	lighting_.setScale(gen_settings_.block_size, gen_settings_.block_size);

	ResetChunks();
	
	// Get current time
	const auto start_time = std::chrono::high_resolution_clock::now();
	
	// Set seeds to current time
	gen_settings_.seed = time(nullptr);
	perlin_noise_.SetSeed(gen_settings_.seed);
	srand(gen_settings_.seed);

	// Set water level
	sea_level_ = gen_settings_.min_sea_level + (rand() % (gen_settings_.max_sea_level - gen_settings_.min_sea_level + 1));

	// Execute all the generation tasks
	for (auto& generation_task : generation_tasks_)
	{
		(*generation_task)(*this);
	}

	// Calculate how long generation took
	const auto end_time = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	generation_time_ = duration.count();

	settings_ = gen_settings_;
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
	const int chunk_x = x / gen_settings_.chunk_width;
	const int chunk_y = y / gen_settings_.chunk_height;

	// Get the chunk
	const Chunk* chunk = chunks_[chunk_x + chunk_y * gen_settings_.num_chunks_x];
	// Convert global coordinates to local chunk coordinates and get the block
	return chunk->GetBlock(x - (chunk_x * gen_settings_.chunk_width), y - (chunk_y * gen_settings_.chunk_height));
}

void World::SetBlock(const int x, const int y, const Block block)
{
	// Don't attempt to set a block if coordinates out of bounds
	if (x < 0 || x >= world_width_ || y < 0 || y >= world_height_)
		return;

	// Calculate the chunk coordinates
	const int chunk_x = x / gen_settings_.chunk_width;
	const int chunk_y = y / gen_settings_.chunk_height;

	// Get the chunk
	Chunk* chunk = chunks_[chunk_x + chunk_y * gen_settings_.num_chunks_x];
	// Convert global coordinates to local chunk coordinates and set the block
	chunk->SetBlock(x - (chunk_x * gen_settings_.chunk_width), y - (chunk_y * gen_settings_.chunk_height), block);
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

void World::UpdateLighting(const sf::View& view)
{
	for (int px = 0; px < world_width_; px++)
	{
		for (int py = world_height_; py >= 0; py--)
		{
			Block block = GetBlock(px, py);
			
			const int base_pixel = (px + (world_height_ - py - 1) * world_width_) * 4;
			if (base_pixel < 0 || base_pixel >= world_width_ * world_height_ * 4)
				continue;

			int distance_to_air = -1;
			
			if (block != blocks::air && GetBlock(px, py + 1) == blocks::air)
				distance_to_air = 1;
			else if (block != blocks::air && GetBlock(px, py + 2) == blocks::air)
				distance_to_air = 2;
			else if (block != blocks::air && GetBlock(px, py + 3) == blocks::air)
				distance_to_air = 3;
			else if(block != blocks::air && GetBlock(px, py + 4) == blocks::air)
				distance_to_air = 4;
			else if (block != blocks::air && GetBlock(px, py + 5) == blocks::air)
				distance_to_air = 5;
			else if (block != blocks::air && GetBlock(px, py + 6) == blocks::air)
				distance_to_air = 6;
			else if (block != blocks::air && GetBlock(px, py + 7) == blocks::air)
				distance_to_air = 7;
			else if (block != blocks::air && GetBlock(px, py + 8) == blocks::air)
				distance_to_air = 8;
			else if (block != blocks::air && GetBlock(px, py + 9) == blocks::air)
				distance_to_air = 9;
			else if (block != blocks::air && GetBlock(px, py + 10) == blocks::air)
				distance_to_air = 10;
			
			const int light_value = distance_to_air > -1 ? maths::Remap(
				distance_to_air,
				0, 10,
				0, 255
			) : 255;
			
			lighting_pixels_[base_pixel] = 0;
			lighting_pixels_[base_pixel + 1] = 0;
			lighting_pixels_[base_pixel + 2] = 0;
			lighting_pixels_[base_pixel + 3] = block == blocks::air ? 0 :
			light_value;
		}
	}

	lighting_tex_.update(lighting_pixels_);
}

void World::Draw(sf::RenderWindow& window)
{
	UpdateLighting(window.getView());
	
	// Calculate the center chunk
	const sf::Vector2f view_center = window.getView().getCenter();
	const int center_chunk_x = view_center.x / settings_.chunk_width / 2;
	const int center_chunk_y = settings_.num_chunks_y - (view_center.y / settings_.chunk_height / 2);

	// Render chunks around the center, in a given square range
	// TODO: Use separate render ranges for x and y due to 16:9 ratio
	for (int x = center_chunk_x - render_range_; x <= center_chunk_x + render_range_; x++)
	{
		for (int y = center_chunk_y - render_range_; y <= center_chunk_y + render_range_; y++)
		{
			if (x < 0 || x >= settings_.num_chunks_x ||
				y < 0 || y >= settings_.num_chunks_y)
				continue;

			window.draw(*chunks_[x + y * settings_.num_chunks_x]);
		}
	}

	window.draw(lighting_);
}
