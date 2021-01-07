#pragma once
#include "Chunk.h"

class World
{
public:
	World(
		int chunk_width, int chunk_height,
		int num_chunks_x, int num_chunks_y,
		int block_size
	);
	~World() = default;

	void AddGenerationStep(void (*step)(World& world));
	void Generate();

	void SetBlock(int x, int y, Block block);
	
	void Draw(sf::RenderWindow& window);

public:
	const int chunk_width_;
	const int chunk_height_;
	const int num_chunks_x_;
	const int num_chunks_y_;
	const int world_width_;
	const int world_height_;
	
	const int block_size_;

private:
	std::vector<void (*)(World& world)> generation_steps_;
	std::vector<Chunk*> chunks_;
};
