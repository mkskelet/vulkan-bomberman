#include "Material.h"
#include "TextureDatabase.h"
#include "VulkanApp.h"

Material::Material(Shader* shader, Texture* texture)
{
	this->shader = shader;
	this->texture = texture;

	VulkanApp::GetRenderer()->CreateDescriptorSets(&texture->textureImageView, &texture->textureSampler, descriptorSetIndex);
}

Material::~Material()
{
	// TODO free descriptor sets


	TextureDatabase::GetInstance().ReleaseTexture(texture);
}