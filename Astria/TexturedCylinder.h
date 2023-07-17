#pragma once
#include "ObjectBase.h"

class TexturedCylinder : public ObjectBase<TexturedCylinder>
{
public:
	TexturedCylinder(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_int_distribution<int>& latDist,
		std::uniform_int_distribution<int>& longDist);
};

