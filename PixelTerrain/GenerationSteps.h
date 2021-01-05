﻿#pragma once
#include "World.h"

namespace generation_steps
{
	/*
	 * Generates a height-map
	 */
	inline void HeightMap(World* world)
	{
        world->perlin_noise_.SetFrequency(1);
    	
		// Height-map generation
        for (int x = 0; x < world->world_width_; x++)
        {
            const int surface_height = maths::Remap(
                world->GetNoise(x, 0),
                -1, 1,
                world->min_surface_level_, world->max_surface_level_);

            const int grass_height = maths::Remap(
				world->GetNoise(x, 10000),
                -1, 1,
                surface_height - world->grass_layer_height_, surface_height);

            const int dirt_height = maths::Remap(
                world->GetNoise(x, 20000),
                -1, 1,
                grass_height - world->dirt_layer_height_, grass_height - 5);
        	
            for (int y = 0; y < world->world_height_; y++)
            {
                if (y >= grass_height && y < surface_height)
                {
                    world->SetBlock(x, y, blocks::grass);

                	// Update surface levels
                    if (y > world->max_surface_level_)
                        world->max_surface_level_ = y;

                    if (y < world->min_surface_level_)
                        world->min_surface_level_ = y;
                } else if (y >= dirt_height && y < grass_height)
                {
                    world->SetBlock(x, y, blocks::dirt);
                } else if (y < dirt_height)
                {
                    world->SetBlock(x, y, blocks::stone);
                }
                else
                {
                    world->SetBlock(x, y, blocks::air);
                } 
            }
        }

		// Smoothing (Removes any pixels at least 3 air blocks around)
        for (int x = 0; x < world->world_width_; x++)
        {
            for (int y = world->min_surface_level_; y < world->world_height_; y++)
            {
                Block* block = world->GetBlock(x, y);
            	if (block != blocks::grass) continue;

                int num_air = 0;
                if (world->GetBlock(x - 1, y) == blocks::air)
                    num_air++;
                if (world->GetBlock(x + 1, y) == blocks::air)
                    num_air++;
                if (world->GetBlock(x, y + 1) == blocks::air)
                    num_air++;

                if (num_air == 3)
                {
                    world->SetBlock(x, y, blocks::air);
                    break;
                }
            }
        }
	}

    /*
    * Shift each surface pixel left or right by a random amount
    * Reads from the active blocks and writes to a copy
    * Then writes the whole copy to the active blocks
    */
	inline void Overhangs(World* world)
	{
        world->perlin_noise_.SetFrequency(10);
		
        std::vector<Block*> overhang_terrain = world->GetBlocks();

        for (int x = 0; x < world->world_width_; x++) {
            for (int y = world->min_surface_level_; y < world->world_height_; y++) {
                Block* block = world->GetBlock(x, y);

            	// Check if the current block is on the surface
                if (block == blocks::air) continue;
                bool is_surface_block = false;
                if (world->GetBlock(x + 1, y) == blocks::air)
                    is_surface_block = true;
                else if (world->GetBlock(x - 1, y) == blocks::air)
                    is_surface_block = true;
            	
                if (is_surface_block)
                {
                    const int move_amount = maths::Remap(
                        world->GetNoise(x + 20000, -10000),
                        -1, 1,
                        -30, 30
                    );
                	
                    Block* new_block =
                        world->GetBlock(x + maths::Sign(move_amount), y) != blocks::air ?
                        blocks::air : block;
                    if (move_amount >= 0)
                    {
                        for (int i = 0; i < move_amount; i++)
                        {
                            const int pos = x + i + y * world->world_width_;
                            if (pos >= world->world_width_ * world->world_height_) break;
                            overhang_terrain[pos] = new_block;
                        }
                    } else
                    {
                        for (int i = 0; i > move_amount; i--)
                        {
                            const int pos = x + i + y * world->world_width_;
                            if (pos >= world->world_width_ * world->world_height_) break;
                            overhang_terrain[pos] = new_block;
                        }
                    }
                }
            }
        }
		
        world->SetBlocks(overhang_terrain);
	}

	/*
	 * Generates a water plane
	 */
	inline void Water(World* world)
	{
        const auto water_start_time = std::chrono::high_resolution_clock::now();
        const int water_level = rand() % world->max_surface_level_;
		
		// Create the water level
		for (int x = 0; x < world->world_width_; x++)
		{
            for (int y = world->min_surface_level_; y < world->world_height_; y++) {
                if (y > water_level) break;
                if (world->GetBlock(x, y) != blocks::air) continue;
            	
                world->SetBlock(x, y, blocks::water);
            }
		}
        const auto water_end_time = std::chrono::high_resolution_clock::now();
        const auto water_duration = std::chrono::duration_cast<std::chrono::milliseconds>(water_end_time - water_start_time);
        std::cout << "Water generation completed in " << water_duration.count() << "ms" << std::endl;

		// Replace grass and dirt near water with sand
        const auto sand_start_time = std::chrono::high_resolution_clock::now();

        const int water_range = 30;
        for (int x = 0; x < world->world_width_; x++)
        {
            bool water_in_col = false;
            for (int y = world->min_surface_level_ - water_range; y < water_level + water_range; y++) {
                Block* block = world->GetBlock(x, y);
                if (block == blocks::air) break;
                if (block == blocks::grass || block == blocks::dirt)
                {
                    bool within_water_range = false;

                	// Checks all points within a square of half-extends water_range
                    for (int nx = x - water_range; nx <= x + water_range; nx++)
                    {
                        for (int ny = y - water_range; ny <= y + water_range; ny++)
                        {
                        	// Below uses a circle check but it takes an extra 15 seconds
                            // if (pow(nx - x, 2) + pow(ny - y, 2) > pow(water_range, 2)) continue;;
                            if (world->GetBlock(nx, ny) == blocks::water)
                            {
                                within_water_range = true;
                                break;
                            }
                        }

                        if (within_water_range) break;
                    }

                    if (within_water_range)
                    {
                        world->SetBlock(x, y, blocks::sand);
                        water_in_col = true;
                    }
                }
            }

            if (!water_in_col)
                x += water_range - 1;
        }

        const auto sand_end_time = std::chrono::high_resolution_clock::now();
        const auto sand_duration = std::chrono::duration_cast<std::chrono::milliseconds>(sand_end_time - sand_start_time);
        std::cout << "Sand generation completed in " << sand_duration.count() << "ms" << std::endl;
	}
}
