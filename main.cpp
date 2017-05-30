#include <stdio.h>  
#include<iostream>
#include<fstream>
#include<string>

#include<GL\glew.h>
#include<GLFW\glfw3.h>
#include<IL\il.h>

using namespace std;

void start();
void render();
void onDestroy();
GLuint getProgram();
void loadImages(string* fileNames, unsigned int* images);
void prepareTexture(unsigned int* images);
void key_callback(GLFWwindow* window, int key, int scanmode, int action, int mode);

GLuint *texture2D;
GLuint samplers;
GLuint vao, program;
GLuint textureUnit = 0;
GLuint textureUnit2 = 1;
GLFWwindow *window;

int main(int argc, char **argv)
{
	if (!glfwInit())
	{
		return -1;
	}

	window = glfwCreateWindow(1024, 768, "Mix Textures", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		return -1;
	}

	start();

	GLuint program = getProgram();

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		render();

		glfwSwapBuffers(window);

		glfwPollEvents();

	}

	onDestroy();

	return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}


string  getShaderSources(const char * fileName)
{
	ifstream readStream;
	string shaderSource = "";
	string line = "";

	readStream.open(fileName, ios::in);
	if (readStream.is_open())
	{
		while (getline(readStream, line))
		{
			shaderSource += "\n" + line;
		}
	}

	readStream.close();

	return shaderSource;
}


GLuint getProgram()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	string aa = getShaderSources("vertexShader.glsl");
	string bb = getShaderSources("fragmentShader.glsl");

	char const * vertexSource = aa.c_str();
	char const * fragmentSource = bb.c_str();

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
	}

	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	return program;
}



void onDestroy()
{
	glDeleteProgram(program);

	glfwTerminate();
}

void start()
{
	GLfloat texCoords[] =
	{
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

	};

	GLfloat position[] =
	{
		-1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		 1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f

	};



	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffers[2];
	glGenBuffers(2, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	glVertexAttribBinding(0, 0);
	glVertexAttribFormat(0, 4, GL_FLOAT, GL_FALSE, 0);
	glBindVertexBuffer(0, buffers[0], 0, 4 * sizeof(GL_FLOAT));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBindVertexBuffer(1, buffers[1], 0, sizeof(GL_FLOAT) * 2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glVertexAttribBinding(1, 1);
	glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	unsigned int* images = new unsigned int[2];
	string* fileNames = new string[2]{ "Lunamon.gif", "images.png" };
	texture2D = new GLuint[2];

	loadImages(fileNames, images);

	prepareTexture(images);

	program = getProgram();
	
	delete[] fileNames;
	delete[] images;
}


// 通过devIL加载图片
void loadImages(std::string* fileNames, unsigned int* images)
{
	ILboolean success1 = false;
	ILboolean success2 = false;		// 加载图片状态

	ilInit();
	ilGenImages(2, images);

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	

	ilBindImage(images[0]);
	success1 = ilLoadImage((ILstring)fileNames[0].c_str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	ilBindImage(images[1]);
	success2 = ilLoadImage((ILstring)fileNames[1].c_str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	if (!success1)
	{
		ilDeleteImages(1, &images[0]);
		cout << "image error!!!";
	}
	if (!success2)
	{
		ilDeleteImages(1, &images[1]);
		cout << "image error!!!";
	}
}


//textureObject 初始化图片数据
void prepareTexture(unsigned int* images)
{
	int w, h;
	unsigned char* data;

	glGenSamplers(1, &samplers);

	glSamplerParameteri(samplers, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(samplers, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(samplers, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(samplers, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindSampler(textureUnit, samplers);
	glBindSampler(textureUnit2, samplers);

	glGenTextures(2, texture2D);

	for (int i = 0; i < 2; i++)
	{
		ilBindImage(images[i]);
		w = ilGetInteger(IL_IMAGE_WIDTH);
		h = ilGetInteger(IL_IMAGE_HEIGHT);
		data = ilGetData();

		glBindTexture(GL_TEXTURE_2D, texture2D[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	ilBindImage(0);
}


void render()
{
	glUseProgram(program);
	
	glUniform1f(2, sin(glfwGetTime()));

	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture2D[0]);
	glUniform1i(0, textureUnit);
	
	glActiveTexture(GL_TEXTURE0 + textureUnit2);
	glBindTexture(GL_TEXTURE_2D, texture2D[1]);
	glUniform1i(1, textureUnit2);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}
