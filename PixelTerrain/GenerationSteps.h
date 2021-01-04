#pragma once
#include "Terrain.h"

namespace generation_steps
{
	inline void HeightMap(Terrain* terrain)
	{
        terrain->perlin_noise_.SetFrequency(1);
		
        for (int x = 0; x < terrain->width_; x++)
        {
            const int surface_height = math_helpers::Remap(
                terrain->GetNoise(x, 0),
                -1, 1,
                terrain->min_surface_level_, terrain->max_surface_level_);

            const int grass_height = math_helpers::Remap(
				terrain->GetNoise(x, 10000),
                -1, 1,
                surface_height - terrain->grass_layer_height_, surface_height - 5);

            const int dirt_height = math_helpers::Remap(
                terrain->GetNoise(x, 20000),
                -1, 1,
                grass_height - terrain->dirt_layer_height_, grass_height - 5);
        	
            for (int y = 0; y < terrain->height_; y++)
            {
                if (y >= grass_height && y < surface_height)
                {
                    terrain->SetBlock(x, y, blocks::kGrass);
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
	}

    /*
    * Shift each surface pixel left or right by a random amount
    * Reads from the active blocks and writes to a copy
    * Then writes the whole copy to the active blocks
    */
	inline void Overhangs(Terrain* terrain)
	{
        terrain->perlin_noise_.SetFrequency(5);
		
        std::vector<Block> overhang_terrain = terrain->GetBlocks();

        for (int x = 0; x < terrain->width_; x++) {
            const int move_amount = math_helpers::Remap(
                terrain->GetNoise(x + 20000, -10000),
                -1, 1,
                -80, 80
            );
        	
            for (int y = terrain->min_surface_level_; y < terrain->height_; y++) {
                Block block = terrain->GetBlock(x, y);
                if (block != blocks::kAir && terrain->GetBlock(x, y + 1) == blocks::kAir)
                {
                    const Block new_block = move_amount < 0 ? blocks::kAir : block;
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

        terrain->SetBlocks(overhang_terrain);
	}
}
