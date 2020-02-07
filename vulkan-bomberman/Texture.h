#pragma once

#include "VulkanCore.h"

class Texture
{
private:
	uint32_t textureMipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	
public:
	VkImageView textureImageView;
	VkSampler textureSampler;

	Texture(const char* path);
	~Texture();
};

