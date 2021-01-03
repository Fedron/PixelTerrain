#pragma once
#include "Terrain.h"

namespace generation_steps
{
	inline void HeightMap(const Terrain* terrain)
	{
        for (int x = 0; x < terrain->width_; x++)
        {
            const double height = terrain->min_surface_level_ + \
                (terrain->GetNoise(x) + 1) * (terrain->max_surface_level_ - \
                    terrain->min_surface_level_) / 2;

            for (int y = 0; y < terrain->height_; y++)
            {
                const int pos = (x + y * terrain->width_) * 4;

                if (y < height)
                    terrain->SetPixel(x, y, sf::Color::Transparent);
                else
                    terrain->SetPixel(x, y, sf::Color::White);
            }
        }
	}
}
