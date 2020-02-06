#pragma once

#include <glm/glm.hpp>
#include "Color.h"

#define PIVOT_TOPLEFT 0
#define PIVOT_CENTER 1

class Sprite 
{
private:
	glm::vec3 position;		///< Position of sprite on the screen.
	glm::vec2 scale;			///< Size of sprite in pixels.
	int textureID;			///< ID of texture to fill sprite with (0 for no texture).
	int spritePivot;			///< Pivot mode. See the definition.
	Color color;

public:
	Sprite() : position(glm::vec3(0, 0, 0)), textureID(-1), spritePivot(PIVOT_TOPLEFT), color({1.0f, 1.0f, 1.0f , 1.0f }) {};
	Sprite(glm::vec3 position, glm::vec2 scale, int textureID);
	Sprite(glm::vec3 position, glm::vec2 scale, int textureID, int spritePivot);
	~Sprite();
	void SetPosition(glm::vec3 position) { this->position = position; }
	void SetScale(glm::vec2 scale) { this->scale = scale; }
	void SetTextureID(int id) { textureID = id; }
	void SetSpritePivot(int pivot) { spritePivot = (pivot < 0 ? PIVOT_TOPLEFT : (pivot > 1 ? PIVOT_CENTER : pivot)); }
	void SetColor(float r, float g, float b) { color.r = r; color.g = g; color.b = b; }
	void SetColor(float r, float g, float b, float a) { color.r = r; color.g = g; color.b = b; color.a = a; }
	glm::vec3 GetPosition() const { return position; }
	glm::vec2 GetScale() const { return scale; }
	int GetTextureID() const { return textureID; }
	void Render();			///< Method used to render a sprite.
	void Start();			///< Method called after constructing the object.
	void Update();			///< Method called every frame.
};