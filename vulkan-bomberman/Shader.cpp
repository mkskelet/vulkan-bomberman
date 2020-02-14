#include "Shader.h"
#include "VulkanCore.h"
#include <iostream>

std::map<const char*, Shader*> Shader::shaderMap = {};

Shader::Shader(const char* name, const char* fragmentShaderPath, const char* vertexShaderPath)
{
	this->name = name;
	this->fragmentShader = fragmentShaderPath;
	this->vertexShader = vertexShaderPath;

	createGraphicsPipeline(shaderIndex, vertexShaderPath, fragmentShaderPath);

	std::cout << "shader index " << shaderIndex << std::endl;
	shaderMap[name] = this;
}

Shader::~Shader()
{
	// TODO mark graphic pipeline to be destroyed

	shaderMap.erase(name);
}

Shader* Shader::Find(const char* name)
{
	const auto& it = shaderMap.find(name);

	if (it != shaderMap.end())
	{
		return it->second;
	}

	return nullptr;
}