#pragma once
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
				if (y < world.chunk_height_)
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
        world.perlin_noise_.SetFrequency(1);

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
                surface_height - world.grass_layer_height_, surface_height);

        	// Dirt thickness of the column
            const int dirt_height = maths::Remap(
                world.GetNoise(x, 20000),
                -1, 1,
                grass_height - world.dirt_layer_height_, grass_height - 5);

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
                Block block = world.GetBlock(x, y);
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
        world.perlin_noise_.SetFrequency(10);

		// Copy of the terrain to adjust
        std::vector<Block> overhang_terrain = world.GetBlocks();

        for (int x = 0; x < world.world_width_; x++) {
            for (int y = world.min_surface_level_; y < world.world_height_; y++) {
                Block block = world.GetBlock(x, y);

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
                    const Block new_block =
                        world.GetBlock(x + maths::Sign(move_amount), y) != blocks::air ?
                        blocks::air : block;
                	
                    if (move_amount >= 0)
                    {
                    	// Positive for-loop
                        for (int i = 0; i < move_amount; i++)
                        {
                        	// Calculates the position of the block in world-space
                            const int pos = (x + i) + y * world.world_width_;
                            if (pos >= world.world_width_ * world.world_height_) break;
                            overhang_terrain[pos] = new_block;
                        }
                    }
                    else
                    {
                    	// Negative for-loop
                        for (int i = 0; i > move_amount; i--)
                        {
                            // Calculates the position of the block in world-space
                            const int pos = (x + i) + y * world.world_width_;
                            if (pos >= world.world_width_ * world.world_height_) break;
                            overhang_terrain[pos] = new_block;
                        }
                    }
                }
            }
        }

        world.SetBlocks(overhang_terrain);
    }
	
    /**
     * Creates a sea level, adds water, and adds sand around
     * the lakes
     *
     * @param world Reference to the world
     */
    inline void Water(World& world)
    {
		// Get the current system time
        const auto water_start_time = std::chrono::high_resolution_clock::now();
		// Calculate the world water level
        const int water_level = rand() % world.max_surface_level_;

        // Create the lakes
        for (int x = 0; x < world.world_width_; x++)
        {
            for (int y = world.min_surface_level_; y < world.world_height_; y++) {
            	// Don't set if above water level, or not air
                if (y > water_level) break;
                if (world.GetBlock(x, y) != blocks::air) continue;

            	// Set the block to water
                world.SetBlock(x, y, blocks::water);
            }
        }

		// Calculate how long lake generation took
        const auto water_end_time = std::chrono::high_resolution_clock::now();
        const auto water_duration = std::chrono::duration_cast<std::chrono::milliseconds>(water_end_time - water_start_time);
        std::cout << "Water generation completed in " << water_duration.count() << "ms" << std::endl;

        // Get current system time
        const auto sand_start_time = std::chrono::high_resolution_clock::now();

        // Replace grass and dirt near water with sand
        const int water_range = 30;
        for (int x = 0; x < world.world_width_; x++)
        {
            bool water_in_col = false;
            for (int y = world.min_surface_level_ - water_range; y < water_level + water_range; y++) {
                Block block = world.GetBlock(x, y);
                if (block == blocks::air) break;
                if (block == blocks::grass || block == blocks::dirt)
                {
                    bool within_water_range = false;

                    /*
                     * Checks all points within a square of half-extends water_range
                     * to see if the current block is near water
                     */
                    for (int nx = x - water_range; nx <= x + water_range; nx++)
                    {
                        for (int ny = y - water_range; ny <= y + water_range; ny++)
                        {
							// Checks if current block is water
                            if (world.GetBlock(nx, ny) == blocks::water)
                            {
                                within_water_range = true;
                                break;
                            }
                        }

                        if (within_water_range) break;
                    }

                	// If water is near the current block set it to sand
                    if (within_water_range)
                    {
                        world.SetBlock(x, y, blocks::sand);
                        water_in_col = true;
                    }
                }
            }

        	/*
        	 * If the column that was just checked didn't have any water
        	 * in it, move the x by the water range to skip checking
        	 * unnecessary blocks
        	 */
            if (!water_in_col)
                x += water_range - 1;
        }

		// Calculates how long the sand generation took
        const auto sand_end_time = std::chrono::high_resolution_clock::now();
        const auto sand_duration = std::chrono::duration_cast<std::chrono::milliseconds>(sand_end_time - sand_start_time);
        std::cout << "Sand generation completed in " << sand_duration.count() << "ms" << std::endl;
    }
}