#include "Random.h"

Random::Random(const int seed) :
mt_generator_(seed)
{
	perlin_.SetSeed(seed);
}

void Random::SetSeed(const int seed)
{
	mt_generator_.seed(seed);
	perlin_.SetSeed(seed);
}

void Random::SetPerlinFrequency(const double freq)
{
	perlin_.SetFrequency(freq);
}

double Random::GetPerlin(const int x, const int y) const
{
	return perlin_.GetValue(x * 0.001, y * 0.001, 0);
}

int Random::GetNumber(const int min, const int max)
{
	return std::uniform_int_distribution<int>{ min, max }(mt_generator_);
}

bool Random::GetBool() const
{
	static auto bool_gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());
	return bool_gen();
}
