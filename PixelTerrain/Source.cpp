#include <noise/noise.h>
#include <SFML/Graphics.hpp>

int main()
{
    // Config variables
    const int terrain_width = 800;
    const int terrain_height = 600;

    const int min_surface_level = 100;
    const int max_surface_level = 500;
	
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Terrain");

    auto* terrain_pixels = new sf::Uint8[terrain_width * terrain_height * 4];
    memset(terrain_pixels, 255, terrain_width * terrain_height * 4); // Set every pixel to white
    sf::Texture terrain_texture;
    terrain_texture.create(terrain_width, terrain_height);

    sf::Sprite terrain_sprite(terrain_texture);

    noise::module::Perlin perlin_noise;
    perlin_noise.SetSeed(time(nullptr));

    for (int x = 0; x < terrain_width; x++)
    {
        double noise_value = perlin_noise.GetValue(x * 0.001, 0, 0);
        double height = min_surface_level + (noise_value + 1) * (max_surface_level - min_surface_level) / 2;
    	
        for (int y = 0; y < terrain_height; y++)
        {
            int pos = (x + y * terrain_width) * 4;

            if (y < height)
                terrain_pixels[pos] = terrain_pixels[pos + 1] = terrain_pixels[pos + 2] = 0;
            else
                terrain_pixels[pos] = terrain_pixels[pos + 1] = terrain_pixels[pos + 2] = 255;

            terrain_pixels[pos + 3] = 255; // Alpha
        }
    }
	
    terrain_texture.update(terrain_pixels);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                // Regenerate terrain
                if (event.key.code == sf::Keyboard::Space)
                {
                    perlin_noise.SetSeed(time(nullptr));
                    for (int x = 0; x < terrain_width; x++)
                    {
                        double noise_value = perlin_noise.GetValue(x * 0.001, 0, 0);
                        double height = min_surface_level + (noise_value + 1) * (max_surface_level - min_surface_level) / 2;

                        for (int y = 0; y < terrain_height; y++)
                        {
                            int pos = (x + y * terrain_width) * 4;

                            if (y < height)
                                terrain_pixels[pos] = terrain_pixels[pos + 1] = terrain_pixels[pos + 2] = 0;
                            else
                                terrain_pixels[pos] = terrain_pixels[pos + 1] = terrain_pixels[pos + 2] = 255;

                            terrain_pixels[pos + 3] = 255; // Alpha
                        }
                    }
                    terrain_texture.update(terrain_pixels);
                }
                break;
            }
        }

        window.clear();
        window.draw(terrain_sprite);
        window.display();
    }

    return 0;
}