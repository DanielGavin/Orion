#ifndef FONT_H
#define FONT_H

#undef STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include <vector>


namespace Orion {

    struct FontChars {
        stbtt_packedchar chars[96];
    };

    //Font class is responsible for baking individual truetype font into bitmaps for different sizes. 
    //TODO(Currently the sizes are decided statically, but might make it dynamic if need be.)
    class Font {
    public:
        explicit Font(void* data);

        unsigned int width();
        unsigned int height();
        unsigned char* bitmap();

        stbtt_aligned_quad quadAsciiCharacter(float* x, float* y, const char& c, const unsigned int& size);
    private:
        unsigned char* m_bakedBitmaps;
        unsigned int m_width;
        unsigned int m_height;
        std::vector<FontChars> m_fontChars;
        std::vector<unsigned int> m_fontSizes;
    };

}

#endif