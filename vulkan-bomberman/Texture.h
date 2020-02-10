#pragma once

#include "VulkanCore.h"

class Texture
{
private:
	uint32_t textureMipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	const char* path;
	
public:
	VkImageView textureImageView;
	VkSampler textureSampler;

	Texture(const char* path);
	~Texture();
	const char* GetPath() const { return path; }
};

