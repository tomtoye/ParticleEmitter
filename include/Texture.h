#pragma once

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

class Texture
{
public:
	Texture();
	Texture(const char* a_pFileName);
	Texture(const Texture& copy);
	~Texture();

	unsigned int	GetID() {return m_ID;}
	unsigned int	Load(const char* fileName);
	bool 			Unload();
	void			GetDimensions(unsigned int& a_width, unsigned int& a_height);
	const char*		GetFilename() { return m_pFilename; }

	void			IncrementReferenceCount() { ++m_refCount; }
	void			DecrementReferenceCount() { --m_refCount; }

private:

	void SetFilename(const char* a_pFilename, bool bLoadTexture = false );

	char* m_pFilename;
	unsigned int m_ID;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_bitsPerPixel;

	int m_refCount;
};

#endif //__TEXTURE_H__
