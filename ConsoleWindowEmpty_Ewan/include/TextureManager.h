#pragma once
#include <map>
#include <string>
//Forward declare Texture as only a pointer will be needed here
//and this avoids cyclic dependency
class Texture;

class TextureManager
{
public:
	//This manager class will act as a Singleton object for ease of access

	static TextureManager* CreateInstance();
	static TextureManager* GetInstance();
	static void DestroyInstance();

	bool TextureExists(const char* a_pName);
	//Load a Texture file --> Calls Texture::Load()
	unsigned int LoadTexture(const char* a_pfilename);
	unsigned int GetTexture(const char* a_filename);

	void ReleaseTexture(unsigned int a_texture);

private:

	static TextureManager* m_instance;

	//References count indicates how many pointers are
	//currently pointing to this texture -> only unload at 0 refs
	typedef struct TextureRef
	{
		Texture* pTexture;
		unsigned int refCount;
	}TextureRef;

	std::map<std::string, TextureRef> m_pTextureMap;

	TextureManager();
	~TextureManager();
};