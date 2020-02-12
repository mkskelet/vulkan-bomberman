#pragma once

#include "Color.h"
#include "Material.h"
#include <map>

class Sprite 
{
private:
	glm::vec3 position;		///< Position of sprite on the screen.
	glm::vec2 scale;			///< Size of sprite in pixels.
	glm::vec2 pivot;			///< Pivot mode. See the definition.
	glm::vec2 tiling;			///< Pivot mode. See the definition.
	Color color;
	Material* material;
	static std::map<Material*, std::vector<Sprite*>> spriteMap;
	bool visible = true;

public:
	Sprite() : position(glm::vec3(0, 0, 0)), material(nullptr), pivot(glm::vec2(0.5f, 0.5f)), color({ 1.0f, 1.0f, 1.0f , 1.0f }), scale(glm::vec2(1.0f, 1.0f)), tiling(glm::vec2(1.0f, 1.0f)) { };
	Sprite(glm::vec3 position, glm::vec2 scale);
	Sprite(glm::vec3 position, glm::vec2 scale, glm::vec2 pivot);
	~Sprite();
	void SetPosition(glm::vec3 position) { this->position = position; }
	void SetScale(glm::vec2 scale) { this->scale = scale; }
	void SetMaterial(Material* material);
	void SetSpritePivot(glm::vec2 pivot) { this->pivot = pivot; }
	void SetSpriteTiling(glm::vec2 tiling) { this->tiling = tiling; }
	inline void SetColor(float r, float g, float b) { color.r = r; color.g = g; color.b = b; }
	inline void SetColor(float r, float g, float b, float a) { color.r = r; color.g = g; color.b = b; color.a = a; }
	glm::vec3 GetPosition() const { return position; }
	glm::vec2 GetScale() const { return scale; }
	glm::vec2 GetPivot() const { return pivot; }
	glm::vec2 GetSpriteTiling() const { return tiling; }
	Color GetColor() const { return color; }
	Material* GetMaterial() const { return material; }
	void SetVisible(bool visible) { this->visible = visible; }
	bool IsVisible() { return visible; }
	void Render();			///< Method used to render a sprite.
	void Start();			///< Method called after constructing the object.
	void Update();			///< Method called every frame.

	static std::map<Material*, std::vector<Sprite*>>& GetSpriteMap() { return spriteMap; }
	static void AddToMap(Sprite* sprite);
	static void RemoveFromMap(Sprite* sprite);
};