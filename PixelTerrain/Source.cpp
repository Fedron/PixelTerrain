#include "GenerationSteps.h"

int main()
{	
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Terrain");

	Terrain terrain(800, 600, 100, 500, 30, 80);
    terrain.AddGenerationStep(generation_steps::HeightMap);
    terrain.Generate();

    int brush_size = 4;
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
                    terrain.Generate();
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
            terrain.SetDirty();
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

        	for (int x = mouse_pos.x - brush_size; x < mouse_pos.x + brush_size; x++)
        	{
                for (int y = mouse_pos.y - brush_size; y < mouse_pos.y + brush_size; y++)
                {
                    terrain.SetBlock(x, terrain.height_ - y, blocks::kDirt);
                }
        	}
        }

    	// Delete blocks
        if (right_mouse_held)
        {
            terrain.SetDirty();
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

            for (int x = mouse_pos.x - brush_size; x < mouse_pos.x + brush_size; x++)
            {
                for (int y = mouse_pos.y - brush_size; y < mouse_pos.y + brush_size; y++)
                {
                    terrain.SetBlock(x, terrain.height_ - y, blocks::kAir);
                }
            }
        }

        terrain.UpdateTexture();
    	
        window.clear();
        window.draw(*terrain.GetSprite());
        window.display();
    }

    return 0;
}