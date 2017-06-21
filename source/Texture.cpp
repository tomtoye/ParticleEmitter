#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SOIL.h"

#include "Texture.h"

#include <iostream>
#include <cstdio>
#include <string.h>
#include <stdlib.h>

Texture::Texture() : m_ID(0), m_pFilename(nullptr), m_width(0), m_height(0), m_bitsPerPixel(0)
{
}

Texture::Texture(const char* a_pFilename) : m_ID(0), m_width(0), m_height(0), m_bitsPerPixel(0)
{
	SetFilename(a_pFilename, true);
	
}

Texture::Texture(const Texture& a_texture)
{
	m_ID = a_texture.m_ID;
	m_width = a_texture.m_width;
	m_height = a_texture.m_height;
	m_bitsPerPixel = a_texture.m_bitsPerPixel;
	SetFilename(a_texture.m_pFilename);
	
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);

	if (m_pFilename != nullptr)
	{
		delete[] m_pFilename;
		m_pFilename = nullptr;
	}
}

void Texture::SetFilename(const char* a_pFilename, bool bSetFilename )
{
	if (a_pFilename != nullptr)
	{
		unsigned long len = strlen(a_pFilename);
		m_pFilename = new char[len + 1];
		strcpy(m_pFilename, a_pFilename);
		if (bSetFilename)
		{
			m_ID = Load(m_pFilename);
		}
	}
}

void Texture::GetDimensions(unsigned int& a_width, unsigned int& a_height)
{
	a_width = m_width;
	a_height = m_height;
}

unsigned int Texture::Load(const char* a_pFilename)
{
	unsigned int texID = 0;
	if (a_pFilename != nullptr)
	{
		if (m_pFilename == nullptr)
		{
			SetFilename(a_pFilename);
		}
		unsigned char* pRawImageData = SOIL_load_image(m_pFilename, (int*)&m_width, (int*)&m_height, (int*)&m_bitsPerPixel, SOIL_LOAD_AUTO);
		if (pRawImageData)
		{
			texID = SOIL_create_OGL_texture(pRawImageData,
											m_width,
											m_height,
											m_bitsPerPixel,
											SOIL_CREATE_NEW_ID,
											SOIL_FLAG_MIPMAPS |
											SOIL_FLAG_INVERT_Y |
											SOIL_FLAG_NTSC_SAFE_RGB |
											SOIL_FLAG_COMPRESS_TO_DXT);
			SOIL_free_image_data(pRawImageData);
			m_ID = texID;
			IncrementReferenceCount();
		}
	}
	if (texID == 0)
	{
		std::cout << "SOIL Image unable to load: " << m_pFilename << std::endl;
	}
	return m_ID;
}

bool Texture::Unload()
{
	DecrementReferenceCount();
	if (m_refCount <= 0)
	{
		return true;
	}
	return false;
}
