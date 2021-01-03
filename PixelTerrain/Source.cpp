#include "GenerationSteps.h"

int main()
{	
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Terrain");

	Terrain terrain(800, 600, 100, 500);
    terrain.AddGenerationStep(generation_steps::HeightMap);
    terrain.Generate();
	
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
                    terrain.Generate();
                break;

            default:
                break;
            }
        }

        window.clear();
        window.draw(*terrain.GetSprite());
        window.display();
    }

    return 0;
}