#pragma once
#pragma warning(disable: 4244 26451)

namespace maths
{
	inline int Remap(const int value, const int min, const int max, const int new_min, const int new_max)
	{
		return new_min + (value - min) * (new_max - new_min) / (max - min);
	}

	inline int Remap(const double value, const int min, const int max, const int new_min, const int new_max)
	{
		return new_min + (value - min) * (new_max - new_min) / (max - min);
	}

	inline int Sign(const int value)
	{
		if (value > 0) return 1;
		if (value < 0) return -1;
		return 0;
	}
}