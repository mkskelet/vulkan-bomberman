#include "Material.h"
#include "TextureDatabase.h"

Material::Material(Shader* shader, Texture* texture)
{
	this->shader = shader;
	this->texture = texture;
	this->texture->IncrementRefCount();

	// TODO create descriptor sets


	descriptorSetIndex = -1; // TODO set proper index once we are able to create descriptor sets
}

Material::~Material()
{
	// TODO free descriptor sets


	// free texture
	texture->DecrementRefCount();
	if (texture->GetRefCount() <= 0)
	{
		TextureDatabase::GetInstance().ReleaseTexture(texture);
	}
}