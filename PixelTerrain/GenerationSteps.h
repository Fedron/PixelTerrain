#pragma once
#include "World.h"

namespace generation_steps
{
	inline void Fill(World& world)
	{
		for (int x = 0; x < world.world_width_; x++)
		{
			for (int y = 0; y < world.world_height_; y++)
			{
				if ((y / world.chunk_height_) % 2 == 0)
					world.SetBlock(x, y, blocks::dirt);
			}
		}
	}
}