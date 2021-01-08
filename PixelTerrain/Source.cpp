#include "GenerationSteps.h"

int main()
{	
    sf::RenderWindow window(sf::VideoMode(1280, 640), "Pixel Terrain");

	// Create world
	// TODO: Make world calculate the chunk_size based on num_chunks and block_size
    auto* world = new World(
        32, 32, 20, 10, 2, 50, 320, 8, 30
    );
	
	// Add all generation steps and generate the world
    world->AddGenerationTask(generation_tasks::HeightMap);
    world->AddGenerationTask(generation_tasks::Overhangs);
    world->AddGenerationTask(generation_tasks::Water);
    world->Generate();

	// Game loop
    while (window.isOpen())
    {
		// Event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                // Regenerate world
                if (event.key.code == sf::Keyboard::Space)
                    world->Generate();
                break;

            default:
                break;
            }
        }

    	// Draw calls
        window.clear();
        world->Draw(window);
        window.display();
    }

    return 0;
}
