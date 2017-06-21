#pragma once
#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include "Manager.h"
#include <map>
#include <string>

class Texture;

class TextureManager : public Manager<TextureManager>
{
	friend class Manager<TextureManager>;
protected:
	TextureManager();
	virtual ~TextureManager();

public:
	unsigned int LoadTexture(const char* a_pTextureFilename);
	void ReleaseTexture(unsigned int& a_uiTextureID);
	void ReleaseAllTextures();

	void GetTextureDimensions(const unsigned int& a_uiTextureID,
				unsigned int& a_width, unsigned int& a_height);

private:
	std::map< std::string, Texture*> m_pTextureMap;

};

#endif //__TEXTUREMANAGER_H__