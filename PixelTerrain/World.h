#pragma once
#include <chrono>
#include <iostream>
#include "Chunk.h"
#include "noise/noise.h"

class World
{
public:
	/**
	 * Responsible for generating terrain in all the chunks
	 *
	 * @param chunk_width Width of a chunk in blocks
	 * @param chunk_height Height of a chunk in blocks
	 * @param num_chunks_x The number of chunks along the width of the world
	 * @param num_chunks_y The number of chunks along the height of the world
	 * @param block_size Pixel size of each block
	 * @param min_surface_level Minimum y-level (in blocks) at which the surface can generate
	 * @param max_surface_level Maximum y-level (in block) at which the surface can generate
	 * @param grass_layer_height The thickness (in blocks) of the grass layer
	 * @param dirt_layer_height The thickness (in blocks) of the dirt layer
	 */
	World(
		int chunk_width, int chunk_height,
		int num_chunks_x, int num_chunks_y,
		int block_size,
		int min_surface_level, int max_surface_level,
		int grass_layer_height, int dirt_layer_height
	);
	/**
	 * Destructs the world and all the chunks
	 */
	~World();

	/**
	 * Adds a generation task to the list of tasks
	 *
	 * @param task The generation task to add
	 */
	void AddGenerationTask(void (*task)(World& world));

	/**
	 * Generates the world, carrying out all the generation tasks in
	 * the order they were added
	 */
	void Generate();

	/**
	 * Gets the perlin-noise value at the given coordinates
	 *
	 * @param x X coordinate at which to sample
	 * @param y Y coordinate at which to sample
	 */
	double GetNoise(int x, int y) const;

	/**
	 * Gets the block at the given coordinates
	 *
	 * @param x The global x coordinate of the block
	 * @param y The global y coordinate of the block
	 *
	 * @returns The block at the coordinates
	 */
	Block GetBlock(int x, int y) const;

	/**
	 * Sets the block at the given coordinates
	 *
	 * @param x The global x coordinate of the block
	 * @param y The global y coordinate of the block
	 * @param block The block to set the coordinates to
	 */
	void SetBlock(int x, int y, Block block);

	// TODO: Returns blocks in a specific chunk (Currently this is too expensive)
	/**
	 * Gets all the blocks in the world
	 *
	 * @returns A vector containing all the block (row-major)
	 */
	std::vector<Block> GetBlocks() const;

	/**
	 * Sets all the blocks in the world
	 *
	 * @param blocks A list of all the blocks (row-major)
	 */
	void SetBlocks(std::vector<Block> blocks);

	/**
	 * Gets the font loaded by the world
	 *
	 * @returns A reference to the loaded font
	 */
	sf::Font& GetFont();
	
	/**
	 * Draws all the chunks to the window
	 *
	 * @param window The window to draw to
	 */
	void Draw(sf::RenderWindow& window);

public:
	// Width (in blocks) of all chunks
	const int chunk_width_;
	// Height (in blocks) of all chunks
	const int chunk_height_;
	// Number of chunks along the width of the world
	const int num_chunks_x_;
	// Number of chunks along the height of the world
	const int num_chunks_y_;
	// The width of the world in blocks
	const int world_width_;
	// The height of the world in blocks
	const int world_height_;
	// The size of a block in pixels
	const int block_size_;

	// Minimum y-level at which the surface is found
	int min_surface_level_;
	// Maximum y-level at which the surface is found
	int max_surface_level_;
	// The thickness of the grass layer
	const int grass_layer_height_;
	// The thickness of the dirt layer
	const int dirt_layer_height_;

	// libnoise Perlin noise module
	noise::module::Perlin perlin_noise_;

	// Controls the drawing of debug utils
	bool debug_mode_ = false;

private:
	// A list of all the generation tasks
	std::vector<void (*)(World& world)> generation_tasks_;
	// A list of all the instantiated chunks (row-major)
	std::vector<Chunk*> chunks_;

	// Default font for debugging
	sf::Font font_;
};
