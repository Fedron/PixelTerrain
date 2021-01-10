#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

/**
 * Able to load several sprites from a single file
 */
class SpriteSheet
{
public:
	/**
	 * Creates an empty sprite sheet
	 */
	SpriteSheet();

	/**
	 * Creates a sprite sheet and loads the texture
	 */
	explicit SpriteSheet(std::string filename);

	/**
	 * Deletes all sprite pointers
	 */
	~SpriteSheet();

	/**
	 * Loads a texture in the current directory with filename
	 *
	 * @param filename Name of the image file to load
	 */
	void LoadSpriteSheet(std::string filename);

	/**
	 * Creates an individual sprite from texture with the given rect
	 *
	 * @param left The left side of the sprite
	 * @param top The top side of the sprite
	 * @param width The width of the sprite
	 * @param height The height of the sprite
	 * @param scale_factor Scale factor of the sprite
	 */
	void CreateSprite(int left, int top, int width, int height, int scale_factor = 1);

	/**
	 * Creates an individual sprite from texture with the given rect
	 *
	 * @param rect The rect of the sprite
	 * @param scale_factor Scale factor of the sprite
	 */
	void CreateSprite(sf::IntRect rect, int scale_factor = 1);

	/**
	 * Gets the sprite at the given index
	 *
	 * @param index Index of the sprite
	 * @returns A pointer to the sprite
	 */
	sf::Sprite* GetSprite(unsigned int index);

private:
	// Sprite sheet texture
	sf::Texture sprite_sheet_;
	// List of all the sprites
	std::vector<sf::Sprite*> sprites_;
};
