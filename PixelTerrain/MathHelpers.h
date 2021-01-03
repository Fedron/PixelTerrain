#pragma once

namespace math_helpers
{
	inline int Remap(const int value, const int min, const int max, const int new_min, const int new_max)
	{
		return new_min + (value - min) * (new_max - new_min) / (max - min);
	}

	inline int Remap(const double value, const int min, const int max, const int new_min, const int new_max)
	{
		return new_min + (value - min) * (new_max - new_min) / (max - min);
	}
}