#ifndef STYLE_H
#define STYLE_H

#include "DataTypes.h"
#include "Color.h"

namespace Orion {

    class DrawQueue;

    class Style {
    public:
        virtual void drawLabel(DrawQueue* queue, const Vec2<float>& p,
            const unsigned int& size, const char* text, const Color& color) = 0;

        virtual void drawButton(DrawQueue* queue, const Vec2<float>& p1,
            const unsigned int& width, const unsigned int& height, const char* text) = 0;
    };

}


#endif