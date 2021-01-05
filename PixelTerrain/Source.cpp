#include "GenerationSteps.h"

int main()
{	
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Terrain");

	World world(80, 60, 10, 10, 100, 500, 20, 80);
	world.AddGenerationStep(generation_steps::HeightMap);
    world.AddGenerationStep(generation_steps::Overhangs);
    world.AddGenerationStep(generation_steps::Water);
    world.Generate();

    int brush_size = 10;
    bool left_mouse_held = false;
    bool right_mouse_held = false;
	
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
                    world.Generate();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                    left_mouse_held = true;
                if (event.mouseButton.button == sf::Mouse::Right)
                    right_mouse_held = true;
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left)
                    left_mouse_held = false;
                if (event.mouseButton.button == sf::Mouse::Right)
                    right_mouse_held = false;
                break;

            default:
                break;
            }
        }

    	// Place dirt block
        if (left_mouse_held)
        {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

        	for (int x = mouse_pos.x - brush_size; x < mouse_pos.x + brush_size; x++)
        	{
                for (int y = mouse_pos.y - brush_size; y < mouse_pos.y + brush_size; y++)
                {
                    world.SetBlock(x, world.world_height_ - y, blocks::sand);
                }
        	}
        }

    	// Delete blocks
        if (right_mouse_held)
        {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

            for (int x = mouse_pos.x - brush_size; x < mouse_pos.x + brush_size; x++)
            {
                for (int y = mouse_pos.y - brush_size; y < mouse_pos.y + brush_size; y++)
                {
                    world.SetBlock(x, world.world_height_ - y, blocks::air);
                }
            }
        }

    	// Update behaviour
        world.Update();

    	// Draw calls
        window.clear();
        world.Draw(&window);
        window.display();
    }

    return 0;
}