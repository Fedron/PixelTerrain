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
	 * @param gen_settings A WorldSettings struct containing all the
	 *     the parameters for generation
	 * @param render_range Default render range to use
	 */
	explicit World(WorldSettings gen_settings, int render_range);
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
	Block* GetBlock(int x, int y) const;

	/**
	 * Sets the block at the given coordinates
	 *
	 * @param x The global x coordinate of the block
	 * @param y The global y coordinate of the block
	 * @param block The block to set the coordinates to
	 */
	void SetBlock(int x, int y, Block* block);

	/**
	 * Updates all the chunks in the view
	 *
	 * @param window The window to draw to
	 */
	void Update(sf::RenderWindow& window);
	
	/**
	 * Draws all the chunks to the window
	 *
	 * @param window The window to draw to
	 */
	void Draw(sf::RenderWindow& window);
#pragma endregion

#pragma region Variables
public:
	/*
	 * Data structure containing settings that were used when the
	 * world was generated.
	 *
	 * These settings can be adjusted from the GUI and so do not contain
	 * current world values
	 */
	WorldSettings gen_settings_;

	/**
	 * Current settings in use by the world
	 */
	WorldSettings settings_;
	// How long (in milliseconds) the previous generation took
	int generation_time_;

	// The half-extends of the square in which chunks are rendered
	int render_range_;

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

private:
	// A list of all the generation tasks
	std::vector<void (*)(World& world)> generation_tasks_;
	// A list of all the instantiated chunks (row-major)
	std::vector<Chunk*> chunks_;
#pragma endregion 
};
