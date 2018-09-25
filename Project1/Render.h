#pragma once

#include "Glew/glew.h"
#include "glm/glm.hpp"

struct vertex
{
	glm::vec4 pos;
	glm::vec4 norm;
	glm::vec2 uv;
};

struct geometry
{
	GLuint vao, vbo, ibo; //buffers
	GLuint size;          //index count

};

struct Shader
{
	GLuint program;
};

struct texture 
{
	GLuint handle;
	unsigned int width, height, channels;
};

geometry makeGeometry(vertex * verts, size_t vertCount,
	unsigned int * indices, size_t indexCount);

void freeGeometry(geometry &geo);

Shader makeShader(const char * vertSource,
	const char * fragSource);

void freeShader(Shader & shad);

texture loadTexture(const char * imagePath);
texture makeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char *pixels);
void freeTexture(texture &tex);

void draw(const Shader & shad, const geometry & geo);

void setUniform(const Shader &shad, GLuint location, const glm::mat4 &value);
void setUniform(const Shader &shad, GLuint location, const texture &value, GLuint textureSlot);
void setUniform(const Shader &shad, GLuint location, const glm::vec3 &value);