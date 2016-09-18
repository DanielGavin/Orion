#include "Font.h"

#define STB_TRUETYPE_IMPLEMENTATION

#include "stb_truetype.h"

namespace Orion {

    Font::Font(void * data)
        : m_fontSizes{ 8, 12, 16, 24, 32, 40, 64 }
    {
        m_width = 1024;
        m_height = 1024;
        m_bakedBitmaps = new unsigned char[m_width * m_height];
        m_fontChars.resize(m_fontSizes.size());

        stbtt_pack_context spc;
        void* alloc_context = nullptr;

        stbtt_PackBegin(&spc, m_bakedBitmaps, m_width, m_height, 0, 1, alloc_context);

        for (auto i = 0; i < m_fontSizes.size(); i++)
        {
            stbtt_PackFontRange(&spc, (unsigned char*)data, 0, m_fontSizes[i], 32, 96, m_fontChars[i].chars);
        }

        stbtt_PackEnd(&spc);

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

    stbtt_aligned_quad Font::quadAsciiCharacter(float* x, float* y, const char& c, const unsigned int& size)
    {
        auto idx = -1;

        for (auto i = 0; i < m_fontSizes.size(); i++)
        {
            if (m_fontSizes[i] == size)
            {
                idx = i;
            }

        }

        assert(idx != -1);


        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(m_fontChars[idx].chars, m_width, m_height, c - 32, x, y, &q, 1);
        return q;
    }

}
