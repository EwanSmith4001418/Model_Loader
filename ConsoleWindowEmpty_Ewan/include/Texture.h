#pragma once
#include <vector>
#include <string>

//A Class to store texture data
//A texture is a data buffer that contains values which relate to pixel colours

class Texture
{
public:
	//constructor + Destructor
	Texture();
	~Texture();

	//Function to load a texture from file
	bool Load(std::string a_filename);
	void unload();
	//get file name
	const std::string& GetFileName() const { return m_filename; }
	unsigned int GetTextureID() const { return m_textureID; }
	void GetDimensions(unsigned int& a_w, unsigned int& a_h) const;

private:
	std::string m_filename;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_textureID;
};

inline void Texture::GetDimensions(unsigned int& a_w, unsigned int& a_h) const
{
	a_w = m_width; a_h = m_height;
}

class CubeMap
{
public:
	CubeMap();
	~CubeMap();

	//Getter function
	unsigned int GetCubeMapTexture() { return m_cubemapTextureID; }

private:
	//Cubemap Load Textures function
	unsigned int LoadCubeMap(std::vector<std::string> faces);

	//Cubemap Variables
	std::vector<std::string> m_skyboxFaces;
	unsigned int m_cubemapTextureID;
};