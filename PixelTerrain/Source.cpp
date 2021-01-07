#include "GenerationSteps.h"

int main()
{	
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pixel Terrain");

    auto* world = new World(
        16, 16, 5, 5, 10
    );
	
    world->AddGenerationStep(generation_steps::Fill);
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
