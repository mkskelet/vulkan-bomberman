#pragma once

#include "Shader.h"
#include "Texture.h"

class Material
{
private:
	Shader* shader;
	Texture* texture;
	int descriptorSetIndex = -1;

public:
	Material(Shader* shader, Texture* texture);
	~Material();
	int GetDescriptorSetIndex() { return descriptorSetIndex; }
};

