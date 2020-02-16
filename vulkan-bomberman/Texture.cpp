#include "Texture.h"
#include "VulkanApp.h"

// don't create textures for use in materials directly, use TextureDatabase::GetTexture
Texture::Texture(const char* path)
{
	//strcpy_s(this->path, sizeof(path), path);
	this->path = path;
	textureMipLevels = 1;
	VulkanApp::GetRenderer()->CreateTextureImage(path, &textureImage, &textureImageMemory);
	VulkanApp::GetRenderer()->CreateTextureImageView(&textureImageView, &textureImage, textureMipLevels);
	VulkanApp::GetRenderer()->CreateTextureSampler(&textureSampler, textureMipLevels);

	// TODO handle texture not found, put placeholder texture
}

Texture::~Texture()
{
	vkDestroySampler(VulkanApp::GetRenderer()->GetDevice(), textureSampler, nullptr);
	vkDestroyImageView(VulkanApp::GetRenderer()->GetDevice(), textureImageView, nullptr);
	vkDestroyImage(VulkanApp::GetRenderer()->GetDevice(), textureImage, nullptr);
	vkFreeMemory(VulkanApp::GetRenderer()->GetDevice(), textureImageMemory, nullptr);
}