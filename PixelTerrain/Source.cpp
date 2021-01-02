#include <noise/noise.h>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Terrain");

    auto* terrain_pixels = new sf::Uint8[800 * 600 * 4];
    sf::Texture terrain_texture;
    terrain_texture.create(800, 600);

    sf::Sprite terrain_sprite(terrain_texture);

    noise::module::Perlin perlinNoise;

    for (int i = 0; i < 800 * 600 * 4; i += 4) {
        terrain_pixels[i] = terrain_pixels[i + 1] = terrain_pixels[i + 2] = 255;
        terrain_pixels[i + 3] = 255;
    }
	
    terrain_texture.update(terrain_pixels);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(terrain_sprite);
        window.display();
    }

    return 0;
}