#include "Bomb.h"
#include "TextureDatabase.h"

Bomb::Bomb() : Sprite(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f), glm::vec2(0.f, 1.f))
{
	Shader* shader = Shader::Find("uber");
	Texture* texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/Bomb.tga");
	Material* material = new Material(shader, texture);
	SetMaterial(material);

	range = 3;
	explosionTime = GameTime::Instance->GetTime() + TIME_TO_EXPLODE;
	remoteFireEnabled = false;
	fire = false;
}

bool Bomb::GetFire() const
{
	if (remoteFireEnabled && fire)
		return true;
	else if (!remoteFireEnabled && explosionTime <= GameTime::Instance->GetTime())
		return true;
	else return false;
}