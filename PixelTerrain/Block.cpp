#include "Block.h"

void Block::operator=(const Block& other)
{
	id = other.id;
	color = other.color;
}

bool Block::operator==(const Block& other) const
{
	return id == other.id;
}

bool Block::operator!=(const Block& other) const
{
	return id != other.id;
}