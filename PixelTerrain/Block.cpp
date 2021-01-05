#include "Block.h"

Block::Block(const int id, const sf::Color color) :
id_(id), color_(color)
{
}

void Block::operator=(const Block& other)
{
	id_ = other.id_;
	color_ = other.color_;
}

bool Block::operator==(const Block& other) const
{
	return id_ == other.id_;
}

bool Block::operator!=(const Block& other) const
{
	return id_ != other.id_;
}

int Block::GetId() const
{
	return id_;
}

sf::Color Block::GetColor() const
{
	return color_;
}
