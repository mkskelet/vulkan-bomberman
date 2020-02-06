#include "Color.h"

glm::vec3 Color::GetRGB()
{
	return glm::vec3(r, g, b);
}

glm::vec4 Color::GetRGBA()
{
	return glm::vec4(r, g, b, a);
}