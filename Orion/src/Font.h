#ifndef FONT_H
#define FONT_H

#undef STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

namespace Orion {

    class Font {
    public:
        Font(void* data);

        unsigned int width();
        unsigned int height();
        unsigned char* bitmap();

        stbtt_aligned_quad quadAsciiCharacter(float* x, float* y, const char& c);
    private:
        unsigned char* m_bakedBitmaps;
        unsigned int m_width;
        unsigned int m_height;
        stbtt_bakedchar m_cdata[96];
    };

}

#endif