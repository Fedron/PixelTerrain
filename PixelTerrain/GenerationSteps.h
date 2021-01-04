#pragma once
#include "Terrain.h"

namespace generation_steps
{
	/*
	 * Generates a height-map
	 */
	inline void HeightMap(Terrain* terrain)
	{
        terrain->perlin_noise_.SetFrequency(1);

		// Height-map generation
        for (int x = 0; x < terrain->width_; x++)
        {
            const int surface_height = math_helpers::Remap(
                terrain->GetNoise(x, 0),
                -1, 1,
                terrain->min_surface_level_, terrain->max_surface_level_);

            const int grass_height = math_helpers::Remap(
				terrain->GetNoise(x, 10000),
                -1, 1,
                surface_height - terrain->grass_layer_height_, surface_height);

            const int dirt_height = math_helpers::Remap(
                terrain->GetNoise(x, 20000),
                -1, 1,
                grass_height - terrain->dirt_layer_height_, grass_height - 5);
        	
            for (int y = 0; y < terrain->height_; y++)
            {
                if (y >= grass_height && y < surface_height)
                {
                    terrain->SetBlock(x, y, blocks::kGrass);

                	// Update surface levels
                    if (y > terrain->max_surface_level_)
                        terrain->max_surface_level_ = y;

                    if (y < terrain->min_surface_level_)
                        terrain->min_surface_level_ = y;
                } else if (y >= dirt_height && y < grass_height)
                {
                    terrain->SetBlock(x, y, blocks::kDirt);
                } else if (y < dirt_height)
                {
                    terrain->SetBlock(x, y, blocks::kStone);
                }
                else
                {
                    terrain->SetBlock(x, y, blocks::kAir);
                } 
            }
        }

		// Smoothing (Removes any pixels at least 3 air blocks around)
        for (int x = 0; x < terrain->width_; x++)
        {
            for (int y = terrain->min_surface_level_; y < terrain->height_; y++)
            {
                Block block = terrain->GetBlock(x, y);
            	if (block != blocks::kGrass) continue;

                int num_air = 0;
                if (terrain->GetBlock(x - 1, y) == blocks::kAir)
                    num_air++;
                if (terrain->GetBlock(x + 1, y) == blocks::kAir)
                    num_air++;
                if (terrain->GetBlock(x, y + 1) == blocks::kAir)
                    num_air++;

                if (num_air == 3)
                {
                    terrain->SetBlock(x, y, blocks::kAir);
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
	inline void Overhangs(Terrain* terrain)
	{
        terrain->perlin_noise_.SetFrequency(10);
		
        std::vector<Block> overhang_terrain = terrain->GetBlocks();

        for (int x = 0; x < terrain->width_; x++) {
            for (int y = terrain->min_surface_level_; y < terrain->height_; y++) {
                Block block = terrain->GetBlock(x, y);

            	// Check if the current block is on the surface
                if (block == blocks::kAir) continue;
                bool is_surface_block = false;
                if (terrain->GetBlock(x + 1, y) == blocks::kAir)
                    is_surface_block = true;
                else if (terrain->GetBlock(x - 1, y) == blocks::kAir)
                    is_surface_block = true;
            	
                if (is_surface_block)
                {
                    const int move_amount = math_helpers::Remap(
                        terrain->GetNoise(x + 20000, -10000),
                        -1, 1,
                        -30, 30
                    );
                	
                    const Block new_block =
                        terrain->GetBlock(x + math_helpers::Sign(move_amount), y) != blocks::kAir ?
                        blocks::kAir : block;
                    if (move_amount >= 0)
                    {
                        for (int i = 0; i < move_amount; i++)
                        {
                            const int pos = x + i + y * terrain->width_;
                            if (pos >= terrain->width_ * terrain->height_) break;
                            overhang_terrain[pos] = new_block;
                        }
                    } else
                    {
                        for (int i = 0; i > move_amount; i--)
                        {
                            const int pos = x + i + y * terrain->width_;
                            if (pos >= terrain->width_ * terrain->height_) break;
                            overhang_terrain[pos] = new_block;
                        }
                    }
                }
            }
        }

        /*for (int x = 0; x < terrain->width_; x++)
        {
	        for (int y = terrain->min_surface_level_; y < terrain->height_ ; y++)
	        {
                Block block = terrain->GetBlock(x, y);
	        	
                if (block != blocks::kGrass) continue;
                bool is_surface_block = false;
	        	
                if (terrain->GetBlock(x, y + 1) == blocks::kAir)
                    is_surface_block = true;
                else if (terrain->GetBlock(x + 1, y) == blocks::kAir)
                    is_surface_block = true;
                else if (terrain->GetBlock(x, y - 1) == blocks::kAir)
                    is_surface_block = true;
                else if (terrain->GetBlock(x - 1, y) == blocks::kAir)
                    is_surface_block = true;
	        	
	        	if (is_surface_block)
	        	{
                    const int move_amount = math_helpers::Remap(
                        terrain->GetNoise(x + 20000, -10000),
                        -1, 1,
                        -30, 30
                    );

                    const int pos = x + move_amount + y * terrain->width_;
	        		if (pos >= terrain->width_ * terrain->height_)
                        continue;
	        		
					overhang_terrain[pos] = blocks::kRed;
	        	}
	        }
        }*/
		
        terrain->SetBlocks(overhang_terrain);
	}

	/*
	 * Generates a water plane
	 */
	inline void Water(Terrain* terrain)
	{
        const auto water_start_time = std::chrono::high_resolution_clock::now();
        const int water_level = rand() % terrain->max_surface_level_;
		
		// Create the water level
		for (int x = 0; x < terrain->width_; x++)
		{
            for (int y = terrain->min_surface_level_; y < terrain->height_; y++) {
                if (y > water_level) break;
                if (terrain->GetBlock(x, y) != blocks::kAir) continue;
            	
                terrain->SetBlock(x, y, blocks::kWater);
            }
		}
        const auto water_end_time = std::chrono::high_resolution_clock::now();
        const auto water_duration = std::chrono::duration_cast<std::chrono::milliseconds>(water_end_time - water_start_time);
        std::cout << "Water generation completed in " << water_duration.count() << "ms" << std::endl;

		// Replace grass and dirt near water with sand
        const auto sand_start_time = std::chrono::high_resolution_clock::now();

        const int water_range = 30;
        for (int x = 0; x < terrain->width_; x++)
        {
            bool water_in_col = false;
            for (int y = terrain->min_surface_level_ - water_range; y < water_level + water_range; y++) {
                const Block block = terrain->GetBlock(x, y);
                if (block == blocks::kAir) break;
                if (block == blocks::kGrass || block == blocks::kDirt)
                {
                    bool within_water_range = false;

                	// Checks all points within a square of half-extends water_range
                    for (int nx = x - water_range; nx <= x + water_range; nx++)
                    {
                        for (int ny = y - water_range; ny <= y + water_range; ny++)
                        {
                        	// Below uses a circle check but it takes an extra 15 seconds
                            // if (pow(nx - x, 2) + pow(ny - y, 2) > pow(water_range, 2)) continue;;
                            if (terrain->GetBlock(nx, ny) == blocks::kWater)
                            {
                                within_water_range = true;
                                break;
                            }
                        }

                        if (within_water_range) break;
                    }

                    if (within_water_range)
                    {
                        terrain->SetBlock(x, y, blocks::kSand);
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
