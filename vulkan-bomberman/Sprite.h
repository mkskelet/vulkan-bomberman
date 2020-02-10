#pragma once

#include "Color.h"
#include "Texture.h"
#include <map>

class Sprite 
{
private:
	glm::vec3 position;		///< Position of sprite on the screen.
	glm::vec2 scale;			///< Size of sprite in pixels.
	Texture* texture;			///< ID of texture to fill sprite with (0 for no texture).
	glm::vec2 pivot;			///< Pivot mode. See the definition.
	glm::vec2 tiling;			///< Pivot mode. See the definition.
	Color color;
	static std::map<Texture*, std::vector<Sprite*>> spriteMap;

public:
	Sprite() : position(glm::vec3(0, 0, 0)), texture(nullptr), pivot(glm::vec2(0.5f, 0.5f)), color({ 1.0f, 1.0f, 1.0f , 1.0f }), scale(glm::vec2(1.0f, 1.0f)), tiling(glm::vec2(1.0f, 1.0f)) { AddToMap(this); };
	Sprite(glm::vec3 position, glm::vec2 scale, Texture* texture);
	Sprite(glm::vec3 position, glm::vec2 scale, Texture* texture, glm::vec2 pivot);
	~Sprite();
	void SetPosition(glm::vec3 position) { this->position = position; }
	void SetScale(glm::vec2 scale) { this->scale = scale; }
	void SetTexture(Texture* texture);
	void SetSpritePivot(glm::vec2 pivot) { this->pivot = pivot; }
	void SetSpriteTiling(glm::vec2 tiling) { this->tiling = tiling; }
	void SetColor(float r, float g, float b) { color.r = r; color.g = g; color.b = b; }
	void SetColor(float r, float g, float b, float a) { color.r = r; color.g = g; color.b = b; color.a = a; }
	glm::vec3 GetPosition() const { return position; }
	glm::vec2 GetScale() const { return scale; }
	glm::vec2 GetPivot() const { return pivot; }
	glm::vec2 GetSpriteTiling() const { return tiling; }
	Color GetColor() const { return color; }
	Texture* GetTexture() const { return texture; }
	void Render();			///< Method used to render a sprite.
	void Start();			///< Method called after constructing the object.
	void Update();			///< Method called every frame.
	static std::map<Texture*, std::vector<Sprite*>>& GetSpriteMap() { return spriteMap; }
	static void AddToMap(Sprite* sprite);
	static void RemoveFromMap(Sprite* sprite);
};