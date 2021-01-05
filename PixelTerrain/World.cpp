#include "World.h"

World::World(
	const int chunk_width, const int chunk_height,
	const int num_chunks_x, const int num_chunks_y,
	const int min_surface_level, const int max_surface_level,
	const int grass_layer_height, const int dirt_layer_height) :
num_chunks_x_(num_chunks_x), num_chunks_y_(num_chunks_y),
world_width_(chunk_width * num_chunks_x), world_height_(chunk_height * num_chunks_y),
chunk_width_(chunk_width), chunk_height_(chunk_height),
min_surface_level_(min_surface_level), max_surface_level_(max_surface_level),
grass_layer_height_(grass_layer_height), dirt_layer_height_(dirt_layer_height)
{
	chunks_.reserve(num_chunks_x * num_chunks_y);
	// Create empty chunks
	for (int chunk_y = 0; chunk_y < num_chunks_x; chunk_y++)
	{
		for (int chunk_x = 0; chunk_x < num_chunks_y; chunk_x++)
		{
			chunks_.emplace_back(new Chunk(this, chunk_x, chunk_y));
		}
	}
}

void World::AddGenerationStep(void(* step)(World* world))
{
	generation_steps_.push_back(step);
}

void World::Generate()
{
	const auto start_time = std::chrono::high_resolution_clock::now();
	perlin_noise_.SetSeed(time(nullptr));

	for (auto& generation_step : generation_steps_)
	{
		(*generation_step)(this);
	}

	const auto end_time = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cout << "Generation completed in " << duration.count() << "ms" << std::endl;
}

double World::GetNoise(const int x, const int y) const
{
	return perlin_noise_.GetValue(x * 0.001, y * 0.001, 0);
}

Block World::GetBlock(const int x, const int y) const
{
	if (x < 0 || x >= world_width_ || y < 0 || y >= world_height_)
		return blocks::kNull;

	const int chunk_x = x / chunk_width_;
	const int chunk_y = y / chunk_height_;
	const Chunk* chunk = chunks_[chunk_x + chunk_y * num_chunks_x_];

	return chunk->GetBlock(x - (chunk_x * chunk_width_), y - (chunk_y * chunk_height_));
}

void World::SetBlock(const int x, const int y, const Block block)
{
	if (x < 0 || x >= world_width_ || y < 0 || y >= world_height_)
		return;

	const int chunk_x = x / chunk_width_;
	const int chunk_y = y / chunk_height_;
	Chunk* chunk = chunks_[chunk_x + chunk_y * num_chunks_x_];

	chunk->SetBlock(x - (chunk_x * chunk_width_), y - (chunk_y * chunk_height_), block);
}

std::vector<Block> World::GetBlocks() const
{
	std::vector<Block> blocks(world_width_ * world_height_, blocks::kNull);
	
	for (int x = 0; x < world_width_; x++)
	{
		for (int y = 0; y < world_height_; y++)
		{
			blocks[x + y * world_width_] = GetBlock(x, y);
		}
	}

	return blocks;
}

void World::SetBlocks(std::vector<Block> blocks)
{
	for (int x = 0; x < world_width_; x++)
	{
		for (int y = 0; y < world_height_; y++)
		{
			SetBlock(x, y, blocks[x + y * world_width_]);
		}
	}
}

void World::Update()
{
	for (Chunk* chunk : chunks_)
	{
		chunk->Update();
	}
}

void World::Draw(sf::RenderWindow* window)
{
	for (Chunk* chunk : chunks_)
	{
		chunk->Draw(window);
	}
}
