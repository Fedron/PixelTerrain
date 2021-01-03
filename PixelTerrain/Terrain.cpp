#include "Terrain.h"

Terrain::Terrain(
	const int width, const int height, const int min_surface_level, const int max_surface_level,
	int grass_layer_height, int dirt_layer_height) :
width_(width), height_(height),
min_surface_level_(min_surface_level), max_surface_level_(max_surface_level),
grass_layer_height_(grass_layer_height), dirt_layer_height_(dirt_layer_height)
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
	const auto start_time = std::chrono::high_resolution_clock::now();
	perlin_noise_.SetSeed(time(nullptr));

	for (auto& generation_step : generation_steps_)
	{
		(*generation_step)(this);
	}

    texture_.update(texture_pixels_);

	const auto end_time = std::chrono::high_resolution_clock::now();	
	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cout << "Generation completed in " << duration.count() << "ms" << std::endl;
}

const sf::Sprite* Terrain::GetSprite() const
{
	return &sprite_;
}

double Terrain::GetNoise(const int x, const int y) const
{
	return perlin_noise_.GetValue(x * 0.001, y * 0.001, 0);
}

void Terrain::SetBlock(const int x, const int y, const Block block) const
{
	const int correct_y = height_ - y - 1;
	const int pos = (x + correct_y * width_) * 4;
	texture_pixels_[pos] = block.color.r;
	texture_pixels_[pos + 1] = block.color.g;
	texture_pixels_[pos + 2] = block.color.b;
	texture_pixels_[pos + 3] = block.color.a;
}
