#pragma once

#include "VulkanCore.h"

class Texture
{
private:
	uint32_t textureMipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

public:
	Texture(const char* path);
	~Texture();
};

