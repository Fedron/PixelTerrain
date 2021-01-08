#pragma once
#include <string>
#include <vector>
#include "Block.h"

class World;

class Chunk : public sf::Drawable
{
public:
	/**
	 * Small individual container of blocks in the world
	 *
	 * @param world Reference to which to bind to
	 * @param world_x The x coordinate of the chunk in the world
	 * @param world_y The y coordinate of the chunk in the world
	 */
	Chunk(World& world, int world_x, int world_y);

	/**
	 * Gets the block at the given coordinates
	 *
	 * @param x The x coordinate, within the chunk, of the block
	 * @param y The y coordinate, within the chunk, of the block
	 *
	 * @returns The block at the coordinates
	 */
	Block GetBlock(int x, int y) const;

	/**
	 * Sets the block at the given coordinates
	 *
	 * @param x The x coordinate, within the chunk, of the block
	 * @param y The y coordinate, within the chunk, of the block
	 * @param block The block to set to
	 */
    void SetBlock(int x, int y, Block block);

private:
	/**
	 * Updates the update text to show the correct information
	 * depending on the state of the chunk
	 */
	void UpdateUpdateText();
	
	/**
	 * SFML draw function, draws to the window
	 */
    void draw(sf::RenderTarget& target, const sf::RenderStates states) const override;

private:
	// Reference to the world in which this chunk is in
    World& world_;
	// X coordinate of the chunk in the world
	const int world_x_;
	// Y coordinate of the chunk in the world
	const int world_y_;

	// State determining if the chunk is being updated
	bool updating_ = false;

	// A list of all the blocks in the chunk (row-major)
	std::vector<Block> blocks_;
	// Tile-map used for drawing the blocks
	sf::VertexArray vertices_;

	// Text used to display chunk's coordinates
	sf::Text coords_text_;
	// Text used to display if the chunk is being updated
	sf::Text update_text_;
};
