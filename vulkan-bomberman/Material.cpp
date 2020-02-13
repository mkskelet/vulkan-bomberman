#include "Material.h"
#include "TextureDatabase.h"

Material::Material(Shader* shader, Texture* texture)
{
	this->shader = shader;
	this->texture = texture;

	// TODO create descriptor sets


	descriptorSetIndex = -1; // TODO set proper index once we are able to create descriptor sets
}

Material::~Material()
{
	// TODO free descriptor sets


	TextureDatabase::GetInstance().ReleaseTexture(texture);
}