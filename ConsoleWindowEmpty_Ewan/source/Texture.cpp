#include "Texture.h"
#include <stb_image.h>
#include <iostream>
#include <glad/glad.h>

//Constructor
Texture::Texture() : m_filename(), m_width(0), m_height(0), m_textureID(0)
{
}
//Destructor
Texture::~Texture()
{
	unload();
}
//Function to load texture from a file
bool Texture::Load(std::string a_filepath)
{
	int width = 0, height = 0, channels = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData = stbi_load(a_filepath.c_str(), &width, &height, &channels, 4);
	if (imageData != nullptr)
	{
		m_filename = a_filepath;
		m_width = width;
		m_height = height;
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(imageData);
		std::cout << "Successfully loaded Image File: " << a_filepath << std::endl;
		return true;
	}
	std::cout << "Failed to open Image File: " << a_filepath << std::endl;
	return false;
}

void Texture::unload()
{
	glDeleteTextures(1, &m_textureID);
}

//CubeMap Constructor & Destructor
CubeMap::CubeMap() : m_skyboxFaces(), m_cubemapTextureID(0)
{
	std::vector<std::string> m_skyboxFaces
	{
		"resource/skymap/right.jpg",
		"resource/skymap/left.jpg",
		"resource/skymap/top.jpg",
		"resource/skymap/bottom.jpg",
		"resource/skymap/front.jpg",
		"resource/skymap/back.jpg"
	};
	m_cubemapTextureID = LoadCubeMap(m_skyboxFaces);
}
CubeMap::~CubeMap()
{
}

//Function to LoadCubeMap textures
unsigned int CubeMap::LoadCubeMap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		const char* filename = (faces[i]).c_str();
		unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			std::cout << "Cubemap Texture loaded at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap Texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}