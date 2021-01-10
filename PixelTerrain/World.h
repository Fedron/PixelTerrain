#pragma once
#include <chrono>
#include <iostream>
#include "Chunk.h"
#include "noise/noise.h"

/**
 * Contains generation settings for the world
 */
struct WorldSettings
{
	int seed;

	int chunk_width;
	int chunk_height;
	int num_chunks_x;
	int num_chunks_y;
	int block_size;

	float surface_smoothness;
	int min_surface_level;
	int max_surface_level;
	int grass_layer_thickness;
	int dirt_layer_thickness;
	float overhang_roughness;
	int overhang_amount;

	bool generate_water;
	int min_sea_level;
	int max_sea_level;
	int sand_range;
};

/**
 * Container for all the chunks
 * Manages the generation of the all the chunks
 */
class World
{
#pragma region Methods
public:
	/**
	 * @param settings A WorldSettings struct containing all the
	 *     the parameters for generation
	 */
	explicit World(WorldSettings settings);
	/**
	 * Destructs the world and all the chunks
	 */
	~World();

	/**
	 * Reallocates memory for chunks to fit new WorldSettings
	 */
	void ResetChunks();
	
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

	// TODO: Make it return blocks in a given rect
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
#pragma endregion

#pragma region Variables
public:
	// Data structure containing settings for generation
	WorldSettings settings_;
	// How long (in milliseconds) the previous generation took
	int generation_time_;

	// Width of the world in blocks
	int world_width_;
	// Height of the world in blocks
	int world_height_;

	// Minimum y-level at which the surface is found
	int min_surface_level_;
	// Maximum y-level at which the surface is found
	int max_surface_level_;
	// The global sea-level
	int sea_level_;

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
#pragma endregion 
};
