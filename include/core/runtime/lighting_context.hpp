#pragma once


namespace Nc
{

/**
* @brief Shader uniform locations and configuration for lighting.
* Holds all shader uniform references related to lighting.
*/
struct LightingContext
{
	int textureLocation = -1;
	int lightSourceCount = 0;
	int lightPositionLocation = -1;
	int lightColorLocation = -1;
	int lightRangeLocation = -1;
	int lightStrengthLocation = -1;
	int lightTypeLocation = -1;
	int lightDirectionLocation = -1;
	int lightAngleLocation = -1;
};

}