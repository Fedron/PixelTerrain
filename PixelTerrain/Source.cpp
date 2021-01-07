#include "GenerationSteps.h"

int main()
{	
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pixel Terrain");

	// TODO: Make world calculate the chunk_size based on num_chunks and block_size
    auto* world = new World(
        32, 32, 20, 10, 2, 10, 120, 8, 30
    );
	
    //world->AddGenerationStep(generation_steps::Fill);
    world->AddGenerationStep(generation_steps::HeightMap);
    world->AddGenerationStep(generation_steps::Overhangs);
    world->AddGenerationStep(generation_steps::Water);
    world->Generate();
	
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
