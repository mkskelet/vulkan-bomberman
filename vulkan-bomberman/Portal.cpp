#include "Portal.h"
#include "TextureDatabase.h"

Portal::Portal() : Sprite(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0, 0), glm::vec2(0.f, 1.f))
{
	Shader* shader = Shader::Find("uber");
	Texture* texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/Portal.tga");
	Material* material = new Material(shader, texture);
	SetMaterial(material);
	unlocked = false;
}