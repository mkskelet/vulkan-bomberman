#include "Powerup.h"
#include "TextureDatabase.h"
#include "Say.h"

Powerup::Powerup() : Sprite(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f), glm::vec2(0.f, 1.f))
{
	SetVisible(false);
	pt = BombP;
}

void Powerup::SetPowerupType(PowerupType pt)
{
	this->pt = pt;
	
	Shader* shader = Shader::Find("uber");
	Texture* texture;
	Material* material;

	switch (pt)
	{
	case PowerupType::BombP:
		texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/BP.tga");
		break;
	case PowerupType::RemoteP:
		texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/RP.tga");
		break;
	case PowerupType::FlameP:
		texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/FP.tga");
		break;
	case PowerupType::SpeedP:
		texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/SP.tga");
		break;
	default:
		Say::Log("Setting powerup texture to NULL");
		texture = nullptr;
	}
	material = new Material(shader, texture);
}