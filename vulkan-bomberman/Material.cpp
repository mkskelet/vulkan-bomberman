#include "Material.h"
#include "TextureDatabase.h"
#include "VulkanApp.h"
#include "Say.h"

Material::Material(Shader* shader, Texture* texture)
{
	this->shader = shader;
	this->texture = texture;

	VulkanApp::GetRenderer()->CreateDescriptorSets(&texture->textureImageView, &texture->textureSampler, descriptorSetIndex);

	Say::LogIf(texture != nullptr, "Create material for", texture->GetPath(), "(mat index:", descriptorSetIndex, ")");
}

Material::~Material()
{
	// TODO free descriptor sets


	TextureDatabase::GetInstance().ReleaseTexture(texture);
}