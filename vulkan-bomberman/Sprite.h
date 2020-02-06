#pragma once

#ifndef INGLUDE_GLM
#define INCLUDE_GLM
#include <glm/glm.hpp>
#endif

#include "Color.h"
#include "Texture.h"

#define PIVOT_TOPLEFT 0
#define PIVOT_CENTER 1

class Sprite 
{
private:
	glm::vec3 position;		///< Position of sprite on the screen.
	glm::vec2 scale;			///< Size of sprite in pixels.
	Texture* texture;			///< ID of texture to fill sprite with (0 for no texture).
	int spritePivot;			///< Pivot mode. See the definition.
	Color color;

public:
	Sprite() : position(glm::vec3(0, 0, 0)), texture(nullptr), spritePivot(PIVOT_TOPLEFT), color({1.0f, 1.0f, 1.0f , 1.0f }) {};
	Sprite(glm::vec3 position, glm::vec2 scale, Texture* texture);
	Sprite(glm::vec3 position, glm::vec2 scale, Texture* texture, int spritePivot);
	~Sprite();
	void SetPosition(glm::vec3 position) { this->position = position; }
	void SetScale(glm::vec2 scale) { this->scale = scale; }
	void SetTexture(Texture *texture) { this->texture = texture; }
	void SetSpritePivot(int pivot) { spritePivot = (pivot < 0 ? PIVOT_TOPLEFT : (pivot > 1 ? PIVOT_CENTER : pivot)); }
	void SetColor(float r, float g, float b) { color.r = r; color.g = g; color.b = b; }
	void SetColor(float r, float g, float b, float a) { color.r = r; color.g = g; color.b = b; color.a = a; }
	glm::vec3 GetPosition() const { return position; }
	glm::vec2 GetScale() const { return scale; }
	Texture* GetTexture() const { return texture; }
	void Render();			///< Method used to render a sprite.
	void Start();			///< Method called after constructing the object.
	void Update();			///< Method called every frame.
};