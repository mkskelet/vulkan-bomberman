#include "Texture.h"

Texture::Texture(const char* path)
{
	textureMipLevels = 1;
	createTextureImage(path, &textureImage, &textureImageMemory);
	createTextureImageView(&textureImageView, &textureImage, textureMipLevels);
	createTextureSampler(&textureSampler, textureMipLevels);

	// TODO handle texture not found, put placeholder texture
}

Texture::~Texture()
{
	vkDestroySampler(VulkanCore::getInstance().device, textureSampler, nullptr);
	vkDestroyImageView(VulkanCore::getInstance().device, textureImageView, nullptr);
	vkDestroyImage(VulkanCore::getInstance().device, textureImage, nullptr);
	vkFreeMemory(VulkanCore::getInstance().device, textureImageMemory, nullptr);
}