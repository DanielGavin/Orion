#include "Font.h"

namespace Orion {

	Font::Font(void * data)
	{
		//test code: currently just setting the baked to 1024*1024 will not fit everyone
		m_width = 1024;
		m_height = 1024;
		m_bakedBitmaps = new unsigned char[m_width * m_height];

		auto ret = stbtt_BakeFontBitmap((const unsigned char*)data, 0, 32.0, m_bakedBitmaps, m_width, m_height, 32, 96, m_cdata);
	}

	unsigned int Font::width()
	{
		return m_width;
	}

	unsigned int Font::height()
	{
		return m_height;
	}

	unsigned char* Font::bitmap()
	{
		return m_bakedBitmaps;
	}

	stbtt_aligned_quad Font::quadAsciiCharacter(float* x, float* y, const char& c)
	{
		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(m_cdata, m_width, m_height, c - 32, x, y, &q, 1);
		return q;
	}

}
