#include "Terrain.h"

Terrain::Terrain(
	const int width, const int height, const int min_surface_level, const int max_surface_level,
	int grass_layer_height, int dirt_layer_height) :
width_(width), height_(height),
min_surface_level_(min_surface_level), max_surface_level_(max_surface_level),
grass_layer_height_(grass_layer_height), dirt_layer_height_(dirt_layer_height),
blocks_(width * height, blocks::kAir)
{
	texture_pixels_ = new sf::Uint8[width * height * 4];
	is_dirty_ = false;
	
	texture_.create(width, height);
	sprite_.setTexture(texture_);
}

Terrain::~Terrain()
{
	delete[] texture_pixels_;
}

void Terrain::AddGenerationStep(void (*step)(Terrain* terrain))
{
	generation_steps_.push_back(step);
}

void Terrain::Generate()
{
	const auto start_time = std::chrono::high_resolution_clock::now();
	is_dirty_ = true;
	perlin_noise_.SetSeed(time(nullptr));

	for (auto& generation_step : generation_steps_)
	{
		(*generation_step)(this);
	}

	const auto end_time = std::chrono::high_resolution_clock::now();	
	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cout << "Generation completed in " << duration.count() << "ms" << std::endl;
}

double Terrain::GetNoise(const int x, const int y) const
{
	return perlin_noise_.GetValue(x * 0.001, y * 0.001, 0);
}

void Terrain::SetBlock(const int x, const int y, const Block block)
{
	if (x < 0 || x >= width_ || y < 0 || y >= height_) return;

	is_dirty_ = true;

	// Set actual block
	blocks_[x + y * width_] = block;

	// Set texture pixel
	const int correct_y = height_ - y - 1;
	const int pos = (x + correct_y * width_) * 4;

	const sf::Color color = block.GetColor();
	texture_pixels_[pos] = color.r;
	texture_pixels_[pos + 1] = color.g;
	texture_pixels_[pos + 2] = color.b;
	texture_pixels_[pos + 3] = color.a;
}

Block Terrain::GetBlock(const int x, const int y) const
{
	if (x < 0 || x >= width_ || y < 0 || y >= height_)
		return blocks::kNull;
	
	return blocks_[x + width_ * y];
}

void Terrain::SetBlocks(std::vector<Block> blocks)
{
	for (int x = 0; x < width_; x++)
	{
		for (int y = 0; y < height_; y++)
		{
			SetBlock(x, y, blocks[x + y * width_]);
		}
	}
}

void Terrain::Update()
{
	for (int x = 0; x < width_; x++)
	{
		for (int y = 1; y < height_; y++)
		{
			Block block = blocks_[x + y * width_];

			// Sand gravity			
			if (block == blocks::kSand && GetBlock(x, y - 1) == blocks::kAir)
			{
				SetBlock(x, y - 1, block);
				SetBlock(x, y, blocks::kAir);
				continue;
			}

			// Water
			if (block == blocks::kWater)
			{
				for (int dy = 0; dy >= -1; dy--)
				{
					for (int dx = -1; dx <= 1; dx++)
					{
						if (GetBlock(x + dx, y + dy) == blocks::kAir)
						{
							SetBlock(x + dx, y + dy, block);
							SetBlock(x, y, blocks::kAir);
						}
					}
				}
			}
		}
	}
}

void Terrain::Draw(sf::RenderWindow* window)
{
	if (is_dirty_)
	{
		is_dirty_ = false;
		texture_.update(texture_pixels_);
	}
	
	window->draw(sprite_);
}
