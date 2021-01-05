#pragma once
#include "Chunk.h"

class World
{
public:
	World(
		int chunk_width, int chunk_height,
		int num_chunks_x, int num_chunks_y,
		int min_surface_level, int max_surface_level,
		int grass_layer_height, int dirt_layer_height);
	~World() = default;
	
	void AddGenerationStep(void (*step)(World* world));
	void Generate();

	double GetNoise(int x, int y) const;

	Block* GetBlock(int x, int y) const;
	void SetBlock(int x, int y, Block* block);

	Block* GetBlockRelativeToChunk(
		int chunk_x, int chunk_y,
		int block_x, int block_y
	) const;
	void SetBlockRelativeToChunk(
		int chunk_x, int chunk_y,
		int block_x, int block_y,
		Block* block
	);
	
	std::vector<Block*> GetBlocks() const;
	void SetBlocks(std::vector<Block*> blocks);
	
	void Update();
	void Draw(sf::RenderWindow* window);

public:
	const int num_chunks_x_;
	const int num_chunks_y_;
	const int world_width_;
	const int world_height_;
	const int chunk_width_;
	const int chunk_height_;
	
	int min_surface_level_;
	int max_surface_level_;
	const int grass_layer_height_;
	const int dirt_layer_height_;

	noise::module::Perlin perlin_noise_;

private:
	std::vector<void (*)(World* world)> generation_steps_;
	std::vector<Chunk*> chunks_;
};
