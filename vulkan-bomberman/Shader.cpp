#include "Shader.h"
#include "VulkanCore.h"
#include <iostream>
#include "VulkanApp.h"
#include "Say.h"

std::map<const char*, Shader*> Shader::shaderMap = {};

Shader::Shader(const char* name, const char* vertexShaderPath, const char* fragmentShaderPath)
{
	this->name = name;
	this->fragmentShader = fragmentShaderPath;
	this->vertexShader = vertexShaderPath;

	VulkanApp::GetRenderer()->CreateGraphicsPipeline(shaderIndex, vertexShaderPath, fragmentShaderPath);

	Say::Log("Add shader", name, shaderIndex);
	shaderMap[name] = this;
}

Shader::~Shader()
{
	Say::Log("Delete shader", name);

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