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

	settings_ = gen_settings_;

	ResetChunks();
	
	// Get current time
	const auto start_time = std::chrono::high_resolution_clock::now();
	
	// Set seeds to current time
	gen_settings_.seed = time(nullptr);
	random_.SetSeed(gen_settings_.seed);
	srand(gen_settings_.seed);

	// Set water level
	sea_level_ = random_.GetNumber(
		gen_settings_.min_sea_level, gen_settings_.max_sea_level
	);

	// Execute all the generation tasks
	for (auto& generation_task : generation_tasks_)
	{
		(*generation_task)(*this);

		for (Chunk* chunk : chunks_)
		{
			chunk->UpdateBlocks();
		}
	}

	// Calculate how long generation took
	const auto end_time = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	generation_time_ = duration.count();
}

Block* World::GetBlock(const int x, const int y) const
{
	// Return null block if out of bounds
	if (x < 0 || x >= world_width_ || y < 0 || y >= world_height_)
		return blocks::null;

	// Calculate the chunk in which block is located
	const int chunk_x = x / settings_.chunk_width;
	const int chunk_y = y / settings_.chunk_height;

	// Get the chunk
	const Chunk* chunk = chunks_[chunk_x + chunk_y * settings_.num_chunks_x];
	// Convert global coordinates to local chunk coordinates and get the block
	return chunk->GetBlock(x - (chunk_x * settings_.chunk_width), y - (chunk_y * gen_settings_.chunk_height));
}

void World::SetBlock(const int x, const int y, Block* block)
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

void World::Update(sf::RenderWindow& window)
{
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

			chunks_[x + y * settings_.num_chunks_x]->Update();
		}
	}
}

void World::Draw(sf::RenderWindow& window)
{
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
}
