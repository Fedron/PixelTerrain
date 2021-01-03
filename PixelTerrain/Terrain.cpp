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

void Terrain::Generate()
{
	perlin_noise_.SetSeed(time(nullptr));
    for (int x = 0; x < width_; x++)
    {
	    const double noise_value = perlin_noise_.GetValue(x * 0.001, 0, 0);
	    const double height = min_surface_level_ + (noise_value + 1) * (max_surface_level_ - min_surface_level_) / 2;

        for (int y = 0; y < height_; y++)
        {
	        const int pos = (x + y * width_) * 4;

            if (y < height)
                texture_pixels_[pos] = texture_pixels_[pos + 1] = \
        		texture_pixels_[pos + 2] = texture_pixels_[pos + 3] = 0;
            else
                texture_pixels_[pos] = texture_pixels_[pos + 1] = \
        		texture_pixels_[pos + 2] = texture_pixels_[pos + 3] = 255;
        }
    }

    texture_.update(texture_pixels_);
}

const sf::Sprite* Terrain::GetSprite() const
{
	return &sprite_;
}
