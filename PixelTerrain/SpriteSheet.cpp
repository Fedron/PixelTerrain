#include "SpriteSheet.h"

SpriteSheet::SpriteSheet()
{
	sprite_sheet_.create(0, 0);
}

SpriteSheet::SpriteSheet(const std::string filename)
{
	LoadSpriteSheet(filename);
}

SpriteSheet::~SpriteSheet()
{
	for (sf::Sprite* sprite : sprites_)
	{
		delete sprite;
	}
}

void SpriteSheet::LoadSpriteSheet(const std::string filename)
{
	if (!sprite_sheet_.loadFromFile(filename))
		throw;
}

void SpriteSheet::CreateSprite(const int left, const int top, const int width, const int height, const int scale_factor)
{
	CreateSprite(sf::IntRect(left, top, width, height), scale_factor);
}

void SpriteSheet::CreateSprite(const sf::IntRect rect, const int scale_factor)
{
	auto* new_sprite = new sf::Sprite(
		sprite_sheet_,
		rect
	);

	new_sprite->setScale(scale_factor, scale_factor);
	
	sprites_.emplace_back(new_sprite);
}

sf::Sprite* SpriteSheet::GetSprite(const unsigned int index)
{
	if (index >= sprites_.size())
		return nullptr;

	return sprites_[index];
}
