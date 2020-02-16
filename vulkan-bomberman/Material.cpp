#include "Material.h"
#include "TextureDatabase.h"
#include "VulkanApp.h"

Material::Material(Shader* shader, Texture* texture)
{
	this->shader = shader;
	this->texture = texture;

	// TODO create descriptor sets
	VulkanApp::GetRenderer()->CreateDescriptorSets(&texture->textureImageView, &texture->textureSampler, descriptorSetIndex);

	descriptorSetIndex = -1; // TODO set proper index once we are able to create descriptor sets
}

Material::~Material()
{
	// TODO free descriptor sets


	TextureDatabase::GetInstance().ReleaseTexture(texture);
}