#include "Sprite.h"
#include "TextureDatabase.h"
#include <iostream>

std::map<Material*, std::vector<Sprite*>> Sprite::spriteMap = {};

/// Constructor to create sprite with specified position, scale and textureID, default pivot PIVOT_CENTER is used.
Sprite::Sprite(glm::vec3 position, glm::vec2 scale) {
	this->position = position;
	this->scale = scale;
	this->material = nullptr;
	pivot = glm::vec2(0.5f, 0.5f);
	tiling = glm::vec2(1.0f, 1.0f);
	color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

/// Constructor to create sprite with specified position, scale and textureID and pivot.
Sprite::Sprite(glm::vec3 position, glm::vec2 scale, glm::vec2 pivot) {
	this->position = position;
	this->scale = scale;
	this->material = nullptr;
	this->pivot = pivot;
	tiling = glm::vec2(1.0f, 1.0f);
	color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

Sprite::~Sprite() 
{
	std::cout << "deleting sprite " << (GetMaterial() == nullptr ? "null" : GetMaterial()->GetTexture()->GetPath()) << std::endl;
	RemoveFromMap(this);
}

void Sprite::SetMaterial(Material* material)
{
	this->material = material;
}

void Sprite::AddToMap(Sprite* sprite)
{
	auto it = spriteMap.find(sprite->GetMaterial());
	if (it != spriteMap.end())
	{
		it->second.push_back(sprite);
		std::cout << "adding reference to sprite map: " << ((it->first == nullptr) ? "null" : it->first->GetTexture()->GetPath()) << std::endl;
	}
	else
	{
		spriteMap[sprite->GetMaterial()].push_back(sprite);
		std::cout << "adding new texture to sprite map: " << ((sprite->GetMaterial() == nullptr) ? "null" : sprite->GetMaterial()->GetTexture()->GetPath()) << std::endl;
	}
}

void Sprite::RemoveFromMap(Sprite* sprite)
{
	auto it = spriteMap.find(sprite->GetMaterial());

	std::cout << "remove from map: " << (sprite->GetMaterial() == nullptr ? "null" : sprite->GetMaterial()->GetTexture()->GetPath()) << std::endl;

	if (it != spriteMap.end())
	{
		auto s = std::find(it->second.begin(), it->second.end(), sprite);

		// remove sprite from map if we found it
		if (s != it->second.end())
		{
			it->second.erase(s);
			std::cout << "erasing reference from sprite map: " << ((sprite->GetMaterial() == nullptr) ? "null" : sprite->GetMaterial()->GetTexture()->GetPath()) << std::endl;
		}

		// release texture if we don't have any references to it
		if (it->second.size() == 0)
		{
			Material* mat = it->first;
			spriteMap.erase(mat);
			//std::cout << "erasing texture from sprite map: " << ((sprite->GetTexture() == nullptr) ? "null" : sprite->GetTexture()->GetPath()) << std::endl;
		}
	}
}

void Sprite::Start() {

}

void Sprite::Update() {

}