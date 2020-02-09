#include "Sprite.h"

/// Constructor to create sprite with specified position, scale and textureID, default pivot PIVOT_CENTER is used.
Sprite::Sprite(glm::vec3 position, glm::vec2 scale, Texture* texture) {
	this->position = position;
	this->scale = scale;
	this->texture = texture;
	pivot = glm::vec2(0.5f, 0.5f);
	tiling = glm::vec2(1.0f, 1.0f);
	color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

/// Constructor to create sprite with specified position, scale and textureID and pivot.
Sprite::Sprite(glm::vec3 position, glm::vec2 scale, Texture* texture, glm::vec2 pivot) {
	this->position = position;
	this->scale = scale;
	this->texture = texture;
	this->pivot = pivot;
	tiling = glm::vec2(1.0f, 1.0f);
	color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

Sprite::~Sprite() 
{
	
}

void Sprite::Render() {
	// set color
	//glColor3f(colorR, colorG, colorB);

	//// prepare for drawing
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBindTexture(GL_TEXTURE_2D, textureID);
	//glEnable(GL_TEXTURE_2D);
	//glBegin(GL_QUADS);

	//// draw textured quad
	//if (spritePivot == PIVOT_TOPLEFT) {
	//	glTexCoord2f(0, 0); glVertex2d(position.X(), position.Y() - scale.Y());
	//	glTexCoord2f(1, 0); glVertex2d(position.X() + scale.X(), position.Y() - scale.Y());
	//	glTexCoord2f(1, 1); glVertex2d(position.X() + scale.X(), position.Y());
	//	glTexCoord2f(0, 1); glVertex2d(position.X(), position.Y());
	//}

	//// end drawing and reset color
	//glEnd();
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);
	//glColor3f(1.0f, 1.0f, 1.0f);
}

void Sprite::Start() {

}

void Sprite::Update() {

}