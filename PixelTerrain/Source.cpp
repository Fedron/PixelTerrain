#include "Chunk.h"
#include "World.h"

int main()
{	
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Pixel Terrain");

    World* world = new World(16, 16, 5, 5, 10);
	
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
        //window.draw(&vertices[0], vertices.size(), sf::Quads);
        world->Draw(window);
        window.display();
    }

    return 0;
}
