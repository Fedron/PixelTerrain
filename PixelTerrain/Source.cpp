#include "GenerationTasks.h"
#include "Gui.h"
#include "SpriteSheet.h"

int main()
{	
    sf::RenderWindow window(sf::VideoMode(1280, 640), "Pixel Terrain");
    ImGui::SFML::Init(window);
	
	// Create world
    auto* world = new World(WorldSettings{
        static_cast<int>(time(nullptr)),
    	32, 32,
    	20, 11,
    	2,
    	1,
    	50, 320,
    	5, 20,
    	10, 30,
    	true,
    	40, 250, 5
    });
	
	// Add all generation steps and generate the world
    world->AddGenerationTask(generation_tasks::HeightMap);
    world->AddGenerationTask(generation_tasks::Overhangs);
    world->AddGenerationTask(generation_tasks::Water);
    world->Generate();

    auto* clouds = new SpriteSheet("Sprites/clouds.png");
    clouds->CreateSprite(1, 27, 29, 10, world->settings_.block_size);
    clouds->CreateSprite(1, 1, 4, 2, world->settings_.block_size);
	
	// Game loop
    sf::Clock delta_clock;
    while (window.isOpen())
    {
		// Event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            default:
                break;
            }
        }

    	// Updates
        ImGui::SFML::Update(window, delta_clock.restart());

    	// GUI
        gui::ShowGenerationSettings(*world);
        //ImGui::ShowDemoWindow();
    	
    	// Draw calls
        window.clear(sf::Color(182, 227, 219, 255));
        world->Draw(window);
        window.draw(*clouds->GetSprite(0));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
