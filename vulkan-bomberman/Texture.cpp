#include "Texture.h"

Texture::Texture(const char* path)
{
	VulkanCore* renderer = VulkanCore::Instance;

	textureMipLevels = 1;
	renderer->createTextureImage(path, &textureImage, &textureImageMemory);
	renderer->createTextureImageView(&textureImageView, &textureImage);
	renderer->createTextureSampler(&textureSampler, textureMipLevels);

	// TODO handle texture not found, put placeholder texture
}

Texture::~Texture()
{
	VulkanCore* renderer = VulkanCore::Instance;

	vkDestroySampler(renderer->device, textureSampler, nullptr);
	vkDestroyImageView(renderer->device, textureImageView, nullptr);
	vkDestroyImage(renderer->device, textureImage, nullptr);
	vkFreeMemory(renderer->device, textureImageMemory, nullptr);
}