#pragma once

#include <map>

class Shader
{
private:
	const char* name;
	const char* fragmentShader;
	const char* vertexShader;
	int shaderIndex = -1; // same as index of RenderPipeline this shader works on
	
	static std::map<const char*, Shader*> shaderMap;

public:
	Shader(const char* name, const char* fragmentShaderPath, const char* vertexShaderPath);
	~Shader();
	int GetShaderIndex() { return shaderIndex; }

	static Shader* Find(const char* name);
};

