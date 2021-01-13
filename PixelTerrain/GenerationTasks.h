#pragma once
#include <functional>
#include <random>
#include "MathHelpers.h"
#include "World.h"

namespace generation_tasks
{
	/**
	 * Used for testing purposes, can fill certain chunks
	 * with specific blocks
	 *
	 * @param world Reference to the world
	 */
	inline void Fill(World& world)
	{
		for (int x = 0; x < world.world_width_; x++)
		{
			for (int y = 0; y < world.world_height_; y++)
			{
				if (y < world.gen_settings_.chunk_height)
                    world.SetBlock(x, y, blocks::dirt);
			}
		}
	}

    /**
     * Generates a height-map
     *
     * @param world Reference to the world
     */
    inline void HeightMap(World& world)
    {
        world.perlin_noise_.SetFrequency(world.gen_settings_.surface_smoothness);

        // Height-map generation
        for (int x = 0; x < world.world_width_; x++)
        {
        	// Random surface height of the column
            const int surface_height = maths::Remap(
                world.GetNoise(x, 0),
                -1, 1,
                world.min_surface_level_, world.max_surface_level_);

        	// Grass thickness of the column
            const int grass_height = maths::Remap(
                world.GetNoise(x, 10000),
                -1, 1,
                surface_height - world.gen_settings_.grass_layer_thickness, surface_height);

        	// Dirt thickness of the column
            const int dirt_height = maths::Remap(
                world.GetNoise(x, 20000),
                -1, 1,
                grass_height - world.gen_settings_.dirt_layer_thickness, grass_height - 5);

            for (int y = 0; y < world.world_height_; y++)
            {
            	// Set the blocks in accordance to the y-level
                if (y >= grass_height && y < surface_height)
                {
                    world.SetBlock(x, y, blocks::grass);

                    // Update surface levels
                    if (y > world.max_surface_level_)
                        world.max_surface_level_ = y;

                    if (y < world.min_surface_level_)
                        world.min_surface_level_ = y;
                }
                else if (y >= dirt_height && y < grass_height)
                {
                    world.SetBlock(x, y, blocks::dirt);
                }
                else if (y < dirt_height)
                {
                    world.SetBlock(x, y, blocks::stone);
                }
                else
                {
                    world.SetBlock(x, y, blocks::air);
                }
            }
        }

        // Smoothing (Removes any pixels at least 3 air blocks around)
        for (int x = 0; x < world.world_width_; x++)
        {
            for (int y = world.min_surface_level_; y < world.world_height_; y++)
            {
                Block* block = world.GetBlock(x, y);
                if (block != blocks::grass) continue;

                int num_air = 0;
                if (world.GetBlock(x - 1, y) == blocks::air)
                    num_air++;
                if (world.GetBlock(x + 1, y) == blocks::air)
                    num_air++;
                if (world.GetBlock(x, y + 1) == blocks::air)
                    num_air++;

                if (num_air == 3)
                {
                    world.SetBlock(x, y, blocks::air);
                    break;
                }
            }
        }
    }

    /**
     * Shifts surface blocks left or right to create overhangs
     * and indents
     *
     * @param world Reference to the world
     */
    inline void Overhangs(World& world)
    {
        world.perlin_noise_.SetFrequency(world.gen_settings_.overhang_roughness);

        for (int x = 0; x < world.world_width_; x++) {
            for (int y = world.min_surface_level_; y < world.world_height_; y++) {
                Block* block = world.GetBlock(x, y);

                // Checks if the current block is on the surface
                if (block == blocks::air || block == blocks::null) continue;
                bool is_surface_block = false;
                if (world.GetBlock(x + 1, y) == blocks::air)
                    is_surface_block = true;
                else if (world.GetBlock(x - 1, y) == blocks::air)
                    is_surface_block = true;

                if (is_surface_block)
                {
                	// Amount to move the block left or right by
                    const int move_amount = maths::Remap(
                        world.GetNoise(x + 20000, -10000),
                        -1, 1,
                        -30, 30
                    );

                	// Determines whether to extend by the block or remove using air
                    Block* new_block =
                        world.GetBlock(x + maths::Sign(move_amount), y) != blocks::air ?
                        blocks::air : block;
                	
                    if (move_amount >= 0)
                    {
                    	// Positive for-loop
                        for (int i = 0; i < move_amount; i++)
                        {
                            if (x + i >= world.world_width_ || y >= world.world_height_) break;

                        	// Calculates the position of the block in world-space
                            const int pos = (x + i) + y * world.world_width_;
                            world.SetBlock(x + i, y, new_block);
                        }
                    }
                    else
                    {
                    	// Negative for-loop
                        for (int i = 0; i > move_amount; i--)
                        {
                            if (x + i < 0 || y < 0) break;
                        	
                            // Calculates the position of the block in world-space
                            const int pos = (x + i) + y * world.world_width_;           
                            world.SetBlock(x + i, y, new_block);
                        }
                    }
                }
            }
        }
    }
	
    /**
     * Creates a sea level, adds water, and adds sand around
     * the lakes
     *
     * @param world Reference to the world
     */
    inline void Water(World& world)
    {
        if (!world.gen_settings_.generate_water)
            return;

        // Create the lakes
        for (int x = 0; x < world.world_width_; x++)
        {
            for (int y = world.min_surface_level_; y < world.world_height_; y++) {
            	// Don't set if above water level, or not air
                if (y > world.sea_level_) break;
                if (world.GetBlock(x, y) != blocks::air) continue;

            	// Set the block to water
                world.SetBlock(x, y, blocks::water);

            	// Set neighbouring grass and dirt blocks to sand
                bool should_break = false;
                for (int nx = x - world.gen_settings_.sand_range; nx <= x + world.gen_settings_.sand_range; nx++)
                {
                    for (int ny = y - world.gen_settings_.sand_range; ny <= y + world.gen_settings_.sand_range; ny++)
                    {
                        Block* block = world.GetBlock(nx, ny);
                        // Checks if current block is water
                        if (block == blocks::grass || block == blocks::dirt)
                        {
                            world.SetBlock(nx, ny, blocks::sand);
                        }
                    }
                }
            }
        }
    }

	/**
	 * Generates trees across the surface
	 *
	 * @param world Reference to the world
	 */
	inline void Trees(World& world)
	{
		// Gets a random bool
        auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());

        // Random number generator
		// TODO: Use PRNG
        std::random_device device;
        std::mt19937 generator(device());
		const std::uniform_int_distribution<int> distribution(10, 25);
		
        const int min_tree_spacing = 5;
        int tree_count = 0;
		
		// Iterate over all surface blocks
		for (int x = 0; x < world.world_width_; x++)
		{
            for (int y = world.min_surface_level_; y <= world.max_surface_level_; y++)
            {
                Block* block = world.GetBlock(x, y);
                if (block != blocks::grass)
                    continue;

                // Checks if the current block is on the surface
                if (block == blocks::air || block == blocks::null) continue;
                bool is_surface_block = false;
                if (world.GetBlock(x + 1, y) == blocks::air)
                    is_surface_block = true;
                else if (world.GetBlock(x - 1, y) == blocks::air)
                    is_surface_block = true;

            	if (is_surface_block)
            	{
                    if (!gen()) continue;
            		
                    const int trunk_height = distribution(generator);
                    const int trunk_width = distribution(generator) / 8;

                    tree_count++;
                    x += min_tree_spacing;

            		// Creates the trunk
            		for (int tx = 0; tx < trunk_width; tx++)
            		{
                        for (int ty = 0; ty < trunk_height; ty++)
                        {
                            world.SetBlock(x + tx, y + ty, blocks::wood);
                        }
            		}

                    // Creates the leaves
                    const int leaves_size = distribution(generator) / 4;
            		for (int lx = x - leaves_size; lx <= x + leaves_size; lx++)
            		{
            			for (int ly = y + trunk_height - leaves_size; ly < y + trunk_height + leaves_size; ly++)
            			{
                            world.SetBlock(lx, ly, blocks::leaf);
            			}
            		}
            		
                    break;
            	}
            }

            if (tree_count >= 20)
                break;
		}
	}
}