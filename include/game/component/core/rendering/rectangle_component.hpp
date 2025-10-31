#pragma once
#include "raylib.h"
#include "core/data/color.hpp"


namespace Component
{
	struct Rectangle
	{
		Nc::RGBa fillColor = Nc::RGBa(WHITE);

		Rectangle() = default;
		Rectangle(Nc::RGBa fillColor): fillColor(fillColor) { };
	};
}