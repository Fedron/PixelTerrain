#include "DayNight.h"
#define PI 3.14159265

DayNight::DayNight(const int gradient_detail, const sf::Vector2i world_size, const int block_size) :
gradient_detail_(gradient_detail),
sky_(sf::Quads, 4)
{
	// Create gradient
	gradient_.insert(0.0, sf::Color(102, 150, 186, 255));
	gradient_.insert(1.0, sf::Color(226, 227, 139, 255));
	gradient_.insert(2.0, sf::Color(231, 165, 83, 255));
	gradient_.insert(3.0, sf::Color(126, 75, 104, 255));
	gradient_.insert(4.0, sf::Color(41, 41, 101, 255));
	gradient_.insert(5.0, sf::Color(7, 11, 52, 255));

	tab_ = new sf::Color[gradient_detail];
	gradient_.fillTab(tab_, gradient_detail);

	day_progress_ = 0.1f;

	// Setup sky vertices
	sky_[0].position = sf::Vector2f(0, 0);
	sky_[1].position = sf::Vector2f(world_size.x, 0);
	sky_[2].position = sf::Vector2f(world_size.x, world_size.y);
	sky_[3].position = sf::Vector2f(0, world_size.y);

	sky_[0].color = sf::Color(102, 150, 186, 255);
	sky_[1].color = sf::Color(102, 150, 186, 255);
	sky_[2].color = sf::Color(102, 150, 186, 255);
	sky_[3].color = sf::Color(102, 150, 186, 255);

	// Load the sun and moon
	sun_texture_.loadFromFile("Sprites/sun.png");
	sun_.setTexture(sun_texture_);
	sun_.setScale(block_size * 2, block_size * 2);
}

void DayNight::Update(float& delta_time)
{
	// Progress the day
	day_progress_ += delta_time * 100 * dir_;
	int target_tab = static_cast<int>(day_progress_);

	// Clamp possible color values in the gradient's range
	if (target_tab >= gradient_detail_)
	{
		dir_ = -1;
		day_progress_ = target_tab = gradient_detail_ - 1;
	} else if (target_tab < 0)
	{
		dir_ = 1;
		day_progress_ = target_tab = 0;
	}

	// Set vertex colours
	int top_target_tab = target_tab - 100;
	if (top_target_tab < 0)
		top_target_tab = 0;

	sky_[0].color = tab_[top_target_tab];
	sky_[1].color = tab_[top_target_tab];
	
	sky_[2].color = tab_[target_tab];
	sky_[3].color = tab_[target_tab];
}

void DayNight::Draw(sf::RenderWindow& window)
{	
	window.draw(sky_);
	//window.draw(sun_);
}
