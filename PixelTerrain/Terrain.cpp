#include "Terrain.h"

Terrain::Terrain(const int width, const int height, const int min_surface_level, const int max_surface_level) :
	width_(width), height_(height),
	min_surface_level_(min_surface_level), max_surface_level_(max_surface_level)
{	
	texture_pixels_ = new sf::Uint8[width * height * 4];
	
	texture_.create(width, height);
	sprite_.setTexture(texture_);
}

Terrain::~Terrain()
{
	delete[] texture_pixels_;
}

void Terrain::AddGenerationStep(void (*step)(const Terrain* terrain))
{
	generation_steps_.push_back(step);
}

void Terrain::Generate()
{
	perlin_noise_.SetSeed(time(nullptr));

	for (int i = 0; i < generation_steps_.size(); i++)
	{
		(*generation_steps_[i])(this);
	}

    texture_.update(texture_pixels_);
}

const sf::Sprite* Terrain::GetSprite() const
{
	return &sprite_;
}

double Terrain::GetNoise(const int x) const
{
	return perlin_noise_.GetValue(x * 0.001, 0, 0);
}

void Terrain::SetPixel(const int x, const int y, const sf::Color color) const
{
	const int pos = (x + y * Terrain::width_) * 4;
	texture_pixels_[pos] = color.r;
	texture_pixels_[pos + 1] = color.g;
	texture_pixels_[pos + 2] = color.b;
	texture_pixels_[pos + 3] = color.a;
}
