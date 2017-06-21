#include "TextureManager.h"
#include "Texture.h"

TextureManager::TextureManager() : Manager<TextureManager>()
{
}

TextureManager::~TextureManager()
{
	ReleaseAllTextures();
}

unsigned int TextureManager::LoadTexture(const char* a_pFilename)
{
	Texture* pTexture = nullptr;
	if (a_pFilename != nullptr)
	{
		std::map<std::string, Texture*>::iterator dictionaryIter = m_pTextureMap.find(a_pFilename);
		if (dictionaryIter != m_pTextureMap.end())
		{
			pTexture = dictionaryIter->second;
			pTexture->IncrementReferenceCount();
		}
		else
		{
			pTexture = new Texture(a_pFilename);
			m_pTextureMap[a_pFilename] = pTexture;
		}
			
	}
	return (pTexture) ? pTexture->GetID() : -1;
}

void TextureManager::ReleaseTexture(unsigned int& a_uiTextureID)
{
	std::map<std::string, Texture*>::iterator dictionaryIter = m_pTextureMap.begin();
	for (; dictionaryIter != m_pTextureMap.end(); ++dictionaryIter)
	{
		Texture* pTexture = reinterpret_cast<Texture*>(dictionaryIter->second);
		if (pTexture != nullptr && pTexture->GetID() == a_uiTextureID)
		{
			if (pTexture->Unload())
			{
				m_pTextureMap.erase(dictionaryIter);
				delete pTexture;
				pTexture = nullptr;
			}
			a_uiTextureID = 0;
			break;
		}
	}
}

void TextureManager::ReleaseAllTextures()
{
	for (auto dictionaryIter = m_pTextureMap.begin(); 
		dictionaryIter != m_pTextureMap.end(); ++dictionaryIter)
	{
		Texture* pTexture = reinterpret_cast<Texture*>(dictionaryIter->second);
		if (pTexture != nullptr)
		{
			pTexture->Unload();
			delete pTexture;
			pTexture = nullptr;
		}
	}
	m_pTextureMap.clear();
}
