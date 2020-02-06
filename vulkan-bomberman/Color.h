#pragma once

#include <glm/glm.hpp>

struct Color
{
public:
	float r;
	float g;
	float b;
	float a;

	glm::vec4 GetRGBA();
	glm::vec3 GetRGB();

	bool operator==(const Color& other) const
	{
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}
};