#include "ColorScale.h"
#include <cmath>
#include <algorithm>

const double PI = 3.1415926535;

double linearInterpolation(double v1, double v2, double mu)
{
	return v1 * (1 - mu) + v2 * mu;
}

double interpolateCosinus(double y1, double y2, double mu)
{
	double mu2;

	mu2 = (1 - cos(mu * PI)) / 2;
	return y1 * (1 - mu2) + y2 * mu2;
}

sf::Color GradientLinear(sf::Color* colorTab, int size, const sf::Vector2f& start, const sf::Vector2f& end, int x, int y)
{
	sf::Vector2f dir = end - start;
	sf::Vector2f pix = sf::Vector2f(x, y) - start;
	double dotProduct = pix.x * dir.x + pix.y * dir.y;
	dotProduct *= (size - 1) / (dir.x * dir.x + dir.y * dir.y);

	if ((int)dotProduct < 0.0) return colorTab[0];
	if ((int)dotProduct > (size - 1)) return colorTab[size - 1];
	return colorTab[(int)dotProduct];
}

sf::Color GradientCircle(sf::Color* colorTab, int size, const sf::Vector2f& start, const sf::Vector2f& end, int x, int y)
{
	sf::Vector2f v_radius = end - start;
	double radius = std::sqrt(v_radius.x * v_radius.x + v_radius.y * v_radius.y);
	sf::Vector2f pix = sf::Vector2f(x, y) - start;
	double dist = std::sqrt(pix.x * pix.x + pix.y * pix.y);
	dist *= (size - 1) / radius;

	if ((int)dist < 0.0) return colorTab[0];
	if ((int)dist > (size - 1)) return colorTab[size - 1];
	return colorTab[(int)dist];
}

sf::Color GradientRadial(sf::Color* colorTab, int size, const sf::Vector2f& start, const sf::Vector2f& end, int x, int y)
{
	sf::Vector2f base = end - start;
	base /= (float)std::sqrt(base.x * base.x + base.y * base.y);
	sf::Vector2f pix = sf::Vector2f(x, y) - start;
	pix /= (float)std::sqrt(pix.x * pix.x + pix.y * pix.y);
	double angle = std::acos(pix.x * base.x + pix.y * base.y);
	double aSin = pix.x * base.y - pix.y * base.x;
	if (aSin < 0) angle = 2 * PI - angle;
	angle *= (size - 1) / (2 * PI);


	if ((int)angle < 0.0) return colorTab[0];
	if ((int)angle > (size - 1)) return colorTab[size - 1];
	return colorTab[(int)angle];
}

sf::Color GradientReflex(sf::Color* colorTab, int size, const sf::Vector2f& start, const sf::Vector2f& end, int x, int y)
{
	sf::Vector2f dir = end - start;
	sf::Vector2f pix = sf::Vector2f(x, y) - start;
	double dotProduct = pix.x * dir.x + pix.y * dir.y;
	dotProduct *= (size - 1) / (dir.x * dir.x + dir.y * dir.y);
	dotProduct = std::abs(dotProduct);

	if ((int)dotProduct < 0.0) return colorTab[0];
	if ((int)dotProduct > (size - 1)) return colorTab[size - 1];
	return colorTab[(int)dotProduct];
}

ColorScale::ColorScale()
{
}

bool ColorScale::insert(double position, sf::Color color)
{
	std::pair< ColorScale::iterator, bool > ret = std::map<double, sf::Color>::insert(std::make_pair(position, color));
	return ret.second;
}


#define ABS(a) (std::max(a, 0))

void ColorScale::fillTab(sf::Color* colorTab, int size, interpolation_function::InterpolationFunction function) const
{

	ColorScale::const_iterator start = std::map<double, sf::Color>::begin();
	ColorScale::const_iterator last = std::map<double, sf::Color>::end();
	last--;

	double pos = 0.0;
	double distance = last->first - start->first;
	ColorScale::const_iterator it = start;

	double(*pFunction)(double, double, double);

	switch (function)
	{
	case interpolation_function::InterpolationFunction::kCosinus: pFunction = interpolateCosinus;  break;
	case interpolation_function::InterpolationFunction::kLinear: pFunction = linearInterpolation; break;
	default: pFunction = interpolateCosinus;  break;

	}
	while (it != last)
	{
		sf::Color startColor = it->second;
		double    startPos = it->first;
		it++;
		sf::Color endColor = it->second;
		double    endPos = it->first;
		double nb_color = ((endPos - startPos) * (double)size / distance);

		for (int i = (int)pos; i <= (int)(pos + nb_color); i++)
		{
			colorTab[i].r = (unsigned char)pFunction(startColor.r, endColor.r, abs((double)i - pos) / (nb_color - 1.0));
			colorTab[i].g = (unsigned char)pFunction(startColor.g, endColor.g, abs((double)i - pos) / (nb_color - 1.0));
			colorTab[i].b = (unsigned char)pFunction(startColor.b, endColor.b, abs((double)i - pos) / (nb_color - 1.0));
			colorTab[i].a = (unsigned char)pFunction(startColor.a, endColor.a, abs((double)i - pos) / (nb_color - 1.0));
		}
		pos += nb_color;
	}

}


void ColorScale::draw(sf::Image& img, const sf::Vector2f& start, const sf::Vector2f& end, gradient_style::GradientStyle style, int size) const
{

	sf::Color(*pFunction)(sf::Color*, int, const sf::Vector2f&, const sf::Vector2f&, int, int);

	sf::Color* tab = new sf::Color[size];
	fillTab(tab, size);

	switch (style)
	{
	case gradient_style::GradientStyle::kLinear: pFunction = GradientLinear; break;
	case gradient_style::GradientStyle::kCircle: pFunction = GradientCircle; break;
	case gradient_style::GradientStyle::kRadial: pFunction = GradientRadial; break;
	case gradient_style::GradientStyle::kReflex: pFunction = GradientReflex; break;

	default: pFunction = GradientLinear;  break;
	}

	for (int i = 0; i < img.getSize().x; i++)
	{
		for (int j = 0; j < img.getSize().y; j++)
		{
			img.setPixel(i, j, pFunction(tab, size, start, end, i, j));
		}
	}
	delete[] tab;
}