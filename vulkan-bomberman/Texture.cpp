#include "Texture.h"

// don't create textures for use in materials directly, use TextureDatabase::GetTexture
Texture::Texture(const char* path)
{
	//strcpy_s(this->path, sizeof(path), path);
	this->path = path;
	textureMipLevels = 1;
	createTextureImage(path, &textureImage, &textureImageMemory);
	createTextureImageView(&textureImageView, &textureImage, textureMipLevels);
	createTextureSampler(&textureSampler, textureMipLevels);

	// TODO handle texture not found, put placeholder texture
}

Texture::~Texture()
{
	vkDestroySampler(VulkanRenderer::GetInstance().device, textureSampler, nullptr);
	vkDestroyImageView(VulkanRenderer::GetInstance().device, textureImageView, nullptr);
	vkDestroyImage(VulkanRenderer::GetInstance().device, textureImage, nullptr);
	vkFreeMemory(VulkanRenderer::GetInstance().device, textureImageMemory, nullptr);
}