#pragma once

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <array>
#include <cstdlib>

struct  Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription getBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
	bool operator==(const Vertex& other) const;
};

class VulkanRenderer
{
public:
	VkQueue graphicsQueue;
	VkCommandPool commandPool;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;

	VkExtent2D swapChainExtent;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	std::vector<VkPipeline> graphicsPipelines;

	static VulkanRenderer& GetInstance();
};

// Global functions
void createTextureImage(const char* path, VkImage* image, VkDeviceMemory* imageMemory, bool useMipMaps = false, uint32_t* mipLevels = nullptr);
void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
void createTextureImageView(VkImageView* imageView, VkImage* image, uint32_t mipLevels);
VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
void createTextureSampler(VkSampler* sampler, uint32_t mipLevels);
VkCommandBuffer beginSingleTimeCommands();
void endSingleTimeCommands(VkCommandBuffer commandBuffer);
void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
bool hasStencilComponent(VkFormat format);
void createGraphicsPipeline(int& index, const char* vertexShaderPath, const char* fragmentShaderPath);
std::vector<char> readFile(const std::string& filename);
VkShaderModule createShaderModule(const std::vector<char>& code);