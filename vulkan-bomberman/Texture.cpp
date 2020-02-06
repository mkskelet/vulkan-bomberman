#include "Texture.h"
#include "main.cpp"

Texture::Texture(const char* path)
{
	VulkanRenderer* renderer = VulkanRenderer::Instance;

	textureMipLevels = 1;
	renderer->createTextureImage(path, &textureImage, &textureImageMemory);
	renderer->createTextureImageView(&textureImageView, &textureImage);
	renderer->createTextureSampler(&textureSampler, textureMipLevels);

	// TODO handle texture not found, put placeholder texture
}

Texture::~Texture()
{
	VulkanRenderer* renderer = VulkanRenderer::Instance;

	vkDestroySampler(renderer->device, textureSampler, nullptr);
	vkDestroyImageView(renderer->device, textureImageView, nullptr);
	vkDestroyImage(renderer->device, textureImage, nullptr);
	vkFreeMemory(renderer->device, textureImageMemory, nullptr);
}