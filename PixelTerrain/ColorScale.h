#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>

namespace interpolation_function
{
	enum InterpolationFunction
	{
		kLinear,
		kCosinus
	};
}

namespace gradient_style
{
	enum GradientStyle
	{
		kLinear,
		kCircle,
		kRadial,
		kReflex
	};
}

class ColorScale : protected std::map<double, sf::Color>
{
public:

	ColorScale();
	bool insert(double position, sf::Color color);

	void fillTab(sf::Color* colorTab, int size, interpolation_function::InterpolationFunction function = interpolation_function::kCosinus) const;
	void draw(sf::Image& img, const sf::Vector2f& start, const sf::Vector2f& end, gradient_style::GradientStyle style = gradient_style::kLinear, int size = 500) const;

};

