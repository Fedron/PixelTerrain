#pragma once
#include "Terrain.h"

namespace generation_steps
{
	inline void HeightMap(const Terrain* terrain)
	{
        for (int x = 0; x < terrain->width_; x++)
        {
            const int surface_height = math_helpers::Remap(
                terrain->GetNoise(x, 0),
                -1, 1,
                terrain->min_surface_level_, terrain->max_surface_level_);

            const int grass_height = math_helpers::Remap(
				terrain->GetNoise(x + 10000, 0),
                -1, 1,
                surface_height - terrain->grass_layer_height_, surface_height - 5);

            const int dirt_height = math_helpers::Remap(
                terrain->GetNoise(x - 10000, 0),
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
}
