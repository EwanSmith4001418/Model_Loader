#include "TextureManager.h"
#include "Texture.h"

//Set up static pointer for Singleton object
TextureManager* TextureManager::m_instance = nullptr;

TextureManager* TextureManager::CreateInstance()
{
	if (nullptr == m_instance)
	{
		m_instance = new TextureManager();
	}
	return m_instance;
}

TextureManager* TextureManager::GetInstance()
{
	if (nullptr == m_instance)
	{
		return TextureManager::CreateInstance();
	}
	return m_instance;
}

void TextureManager::DestroyInstance()
{
	if (nullptr != m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

TextureManager::TextureManager() : m_pTextureMap()
{
}

TextureManager::~TextureManager()
{
	m_pTextureMap.clear();
}

//Use an std map as a texture directory and reference counting
unsigned int TextureManager::LoadTexture(const char* a_filename)
{
	if (a_filename != nullptr)
	{
		auto dictionaryIter = m_pTextureMap.find(a_filename);
		if (dictionaryIter != m_pTextureMap.end())
		{
			//Texture is already in map, increment ref and return texture ID
			TextureRef& texRef = (TextureRef&)(dictionaryIter->second);
			++texRef.refCount;
			return texRef.pTexture->GetTextureID();
		}
		else
		{
			//Texture is not dictionary load in from file
			Texture* pTexture = new Texture();
			if (pTexture->Load(a_filename))
			{
				//Successful Load
				TextureRef texRef = { pTexture, 1 };
				m_pTextureMap[a_filename] = texRef;
				return pTexture->GetTextureID();
			}
			else
			{
				delete pTexture;
				return 0;
			}
		}
	}
	return 0;
}

void TextureManager::ReleaseTexture(unsigned int a_texture)
{
	for (auto dictionaryIter = m_pTextureMap.begin();
		dictionaryIter != m_pTextureMap.end(); ++dictionaryIter)
	{
		TextureRef& texRef = (TextureRef&)(dictionaryIter->second);
		if (a_texture == texRef.pTexture->GetTextureID())
		{
			//Pre decrement will happen prior to this call to ==
			if (--texRef.refCount == 0)
			{
				delete texRef.pTexture;
				texRef.pTexture = nullptr;
				m_pTextureMap.erase(dictionaryIter);
				break;
			}
		}
	}
}

bool TextureManager::TextureExists(const char* a_filename)
{
	auto dictIter = m_pTextureMap.find(a_filename);
	return (dictIter != m_pTextureMap.end());
}

unsigned int TextureManager::GetTexture(const char* a_filename)
{
	auto dictIter = m_pTextureMap.find(a_filename);
	if (dictIter != m_pTextureMap.end())
	{
		TextureRef& texRef = (TextureRef&)(dictIter->second);
		texRef.refCount++;
		return texRef.pTexture->GetTextureID();
	}
	return 0;
}
