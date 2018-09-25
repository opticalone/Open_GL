#include "context.h"
#include "Render.h"
#include "glm/ext.hpp"
#include <cstdio>

int main()
{
	context game;
	game.init(800, 600, "Cheese");

	vertex triVerts[] =
	{
		{ { 1, -.25f, 0, 1 }, {0, 0, 1, 1}, {0, 0} },
	{ { -1, -.25f, 0, 1 }, { 0, 0, 1, 1 }, {0, 1} },
	{ { 0, .25f, 0, 1 },{ 0, 0, 1, 1 }, {0.5f, 1} }
	};
	unsigned int triIndices[] = { 2, 1, 0 };

	//triangles
	geometry triangle = makeGeometry(triVerts, 3, triIndices, 3);

	const char * basicVert =
		"#version 410\n"
		"layout (location = 0) in vec4 position;\n"
		"void main() { gl_Position = position; }";

	const char * mvpVert =
		"#version 430\n"
		"layout (location = 0) in vec4 position;\n"
		"layout (location = 1) in vec4 normal;\n"
		"layout (location = 2) in vec2 uv;\n"
		"layout (location = 0) uniform mat4 proj;\n"
		"layout (location = 1) uniform mat4 view;\n"
		"layout (location = 2) uniform mat4 model;\n"
		"out vec2 vUV;\n"
		"out vec3 vNormal;\n"
		"void main() { gl_Position = proj * view * model * position; vUV = uv; vNormal = normalize(model * normal).xyz; }";

	const char * basicFrag =
		"#version 330\n"
		"out vec4 vertColor;\n"
		"void main() { vertColor = vec4(1.0, 1.0, 1.0, 1.0); }";

	const char * texFrag =
		"#version 430\n"
		"in vec2 vUV;\n"
		"out vec4 vertColor;\n"
		"layout (location = 3) uniform sampler2D albedo;\n"
		"void main() { vertColor = texture(albedo, vUV); }";

	const char * mvpFrag =
		"#version 430\n"
		"layout (location = 3) uniform sampler2D albedo;\n"
		"layout (location = 4) uniform vec3 lightDir;\n"
		"in vec2 vUV;\n"
		"in vec3 vNormal;\n"
		"out vec4 outColor;\n"
		"void main() { float diffuse = max(0, dot(vNormal, -lightDir));\n"
		"outColor = texture(albedo, vUV);\n"
		"outColor = vec4(outColor.x * diffuse, outColor.y * diffuse, outColor.z * diffuse, 1); }";

	Shader basicShad = makeShader(basicVert, basicFrag);
	Shader mvpShad = makeShader(mvpVert, basicFrag);
	Shader texShad = makeShader(mvpVert, texFrag);
	Shader mvpFragShadTest = makeShader(mvpVert, mvpFrag);

	unsigned char whitePixel[] = { 58, 125, 185 };
	texture whiteTexture = makeTexture(1, 1, 3, whitePixel);

	glm::mat4 cam_proj = glm::perspective(glm::radians(45.f), 800.0f / 600.0f, 0.1f, 1000.0f);
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 0, -1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 triangle_model = glm::identity<glm::mat4>();

	glm::vec3 lightDir = glm::vec3(-1, 0, 0);

	//funStuff
	int red = rand() % 255;
	int green = rand() % 255;
	int blue = rand() % 255;

	int rotx = 0;
	int roty = 1;
	int rotz = 0;

	int timerMax = 25;
	int timer = timerMax;

	whitePixel[0] = red;
	whitePixel[1] = green;
	whitePixel[2] = blue;

	texture newTex = makeTexture(1, 1, 3, whitePixel);
	texture textTex = loadTexture("me.jpg");

	while (!game.shouldClose())
	{
		game.tick();
		game.clear();
		
		triangle_model = glm::rotate(triangle_model, glm::radians(1.f), glm::vec3(rotx, roty, rotz));

		setUniform(mvpFragShadTest, 0, cam_proj);
		setUniform(mvpFragShadTest, 1, cam_view);
		setUniform(mvpFragShadTest, 2, triangle_model);
		setUniform(mvpFragShadTest, 3, textTex, 0);
		setUniform(mvpFragShadTest, 4, lightDir);

		draw(mvpFragShadTest, triangle);

		//printf("%d\n", timer);
		timer--;
		if (timer <= 0) 
		{
			red = rand() % 255;
			green = rand() % 255;
			blue = rand() % 255;

			whitePixel[0] = red;
			whitePixel[1] = green;
			whitePixel[2] = blue;
			newTex = makeTexture(1, 1, 3, whitePixel);
			
			timer = timerMax;
		}
	}

	game.term();

	return 0;
}